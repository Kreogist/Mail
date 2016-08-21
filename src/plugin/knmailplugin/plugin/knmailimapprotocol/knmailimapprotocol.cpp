/*
 * Copyright (C) Kreogist Dev Team
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */
#include <QTcpSocket>

#include "knmailmodel.h"
#include "knmailaccount.h"
#include "knmailutil.h"

#include "knmailimapprotocol.h"

KNMailImapProtocol::KNMailImapProtocol(QObject *parent) :
    KNMailReceiverProtocol(parent),
    m_header(QString("*")),
    m_idCode(0),
    m_loginState(false)
{
}

bool KNMailImapProtocol::connectToHost()
{
    //Get the receive config.
    const KNMailProtocolConfig &config=account()->receiveConfig();
    //Connect to host server.
    if(!startConnection(config))
    {
        //Can't connect to server.
        setLastError(ConnectionFailed);
        return false;
    }
    //Wait for server response.
    if(!waitForConnection())
    {
        //Set the last error to be connection timeout.
        setLastError(ConnectionTimeout);
        //Failed to get response.
        return false;
    }
    //After connection, the login state reset to false.
    m_loginState=false;
    //Check the response.
    return waitAndCheckResponse();
}

bool KNMailImapProtocol::login()
{
    //Check the state of pop socket.
    if(socket()->state()!=QAbstractSocket::ConnectedState &&
            //Socket is not online, try to re-connect.
            (!connectToHost()))
    {
        //Failed to connect to host, then failed to login.
        return false;
    }
    //Get the account.
    KNMailAccount *mailAccount=account();
    //Reset the last error.
    setLastError(-1);
    //! FIXME: Replace this with a better authentication.
    //Send login command.
    sendImapMessage("LOGIN "+mailAccount->username() + " " +
                    "\""+mailAccount->password()+"\"");
    //Wait for response.
    if(!waitAndCheckResponse())
    {
        //Username and password is not correct.
        setLastError(LoginPasswordError);
        //Failed to login.
        return false;
    }
    //For netease, we have to use ID command to register the client.
    sendImapMessage("ID (\"name\" \"Kreogist Mail\""
                    "\"version\" \"1.0\""
                    "\"vendor\" \"Kreogist Dev Team\")");
    //Wait for response, we don't need to check the response.
    if(!waitForResponse())
    {
        //Failed to send the ID command.
        return false;
    }
    //Set the login state.
    m_loginState=true;
    //Login success.
    return true;
}

bool KNMailImapProtocol::updateFolderStatus()
{
    //Check the login state.
    if((!m_loginState) && (!login()))
    {
        //Failed to login, then failed to update the status.
        return false;
    }
    //Prepare the cache.
    QList<QByteArray> folderStatus;
    //Get all folder names.
    sendImapMessage("LIST \"\" \"*\"");
    //Wait for response.
    if(!waitAndCheckResponse(&folderStatus))
    {
        //No Response from the cache
        return false;
    }
    //Get the mail account.
    KNMailAccount *mailAccount=account();
    //Get the current mail name list.
    QList<KNMailModel *> customFolders=mailAccount->customFolders(),
                         updatedCustomFolders;
    //Check the folder status.
    for(auto i : folderStatus)
    {
        //Translate all the data.
        QString folderInfo=KNMailUtil::fromUtf7(i);
        //Parse the folder info.
        if(!folderInfo.startsWith("* LIST "))
        {
            //This means that this line doesn't contains information about the
            //folder.
            continue;
        }
        //Remove the LIST.
        folderInfo=folderInfo.mid(7);
        //Find the flag part and name parts.
        int flagEndPosition=folderInfo.indexOf(')');
        if(flagEndPosition==-1)
        {
            //This line cannot be used.
            continue;
        }
        //Get the flag part and name part.
        QString folderFlag=folderInfo.left(flagEndPosition+1);
        //Find the folder position.
        int quoteStart=folderInfo.indexOf('\"', flagEndPosition);
        //If can't find quote, end.
        if(quoteStart==-1)
        {
            continue;
        }
        //Find quote end.
        int quoteEnd=folderInfo.indexOf('\"', quoteStart+1);
        //If can't find end quote, end.
        if(quoteEnd==-1)
        {
            continue;
        }
        //Check directory path, if it is not "\", ignore the folder.
        if(folderInfo.mid(quoteStart, quoteEnd-quoteStart+1)!="\"/\"")
        {
            //Ignore the folder which is not at the root directory.
            continue;
        }
        //Get the folder name.
        QString folderName=folderInfo.mid(quoteEnd+1).simplified();
        //Remove the possible quote.
        if(folderName.size()>2 && folderName.at(0)=='\"')
        {
            //Remove the quote on both side.
            folderName=folderName.mid(1, folderName.size()-2);
        }
        qDebug()<<folderFlag<<folderName;
        //! FIXME: Get folder latest status.
        //Check whether the folder name is exist in the name list.
        if(folderName=="INBOX")
        {
            //Goto Next.
            qDebug()<<"Get: Inbox";
            continue;
        }
        if(folderFlag.contains("\\Sent"))
        {
            //System folder, Sent items.
            qDebug()<<"Get: Sent Items";
            continue;
        }
        if(folderFlag.contains("\\Junk"))
        {
            //System folder, Junk mail.
            qDebug()<<"Get: Junk Mail";
            continue;
        }
        if(folderFlag.contains("\\Trash"))
        {
            //System folder, Trash can.
            qDebug()<<"Get: Trash";
            continue;
        }
        if(folderFlag.contains("\\Drafts"))
        {
            //System folder, drafts.
            qDebug()<<"Get: Drafts";
            continue;
        }
        //Check the folder name in the folder name list.
        bool folderHitFlag=false;
        //Check the folder in original list.
        for(int i=0; i<customFolders.size(); ++i)
        {
            //Check the folder name.
            if(customFolders.at(i)->folderName()==folderName)
            {
                //Hit the folder.
                folderHitFlag=true;
                qDebug()<<"Hit custom folder: "<<folderName;
                //Take the folder from the original folder list, append to new
                //folder list.
                KNMailModel *hitFolder=customFolders.takeAt(i);
                //! FIXME: Add update hit folder status code here.
                updatedCustomFolders.append(hitFolder);
                //Finish loop.
                break;
            }
        }
        //Check the hit flag.
        if(folderHitFlag)
        {
            //Move to next folder.
            continue;
        }
        qDebug()<<"Create custom folder: "<<folderName;
        //We didn't find the folder, we have to construct this folder.
        KNMailModel *folder=new KNMailModel(mailAccount);
        //Set the folder name.
        folder->setFolderName(folderName);
        //Add folder to updated list.
        updatedCustomFolders.append(folder);
    }
    //Clear all the left folder in the original list.
    while(!customFolders.isEmpty())
    {
        //Get one folder.
        KNMailModel *folder=customFolders.takeLast();
        qDebug()<<"Remove folder:"<<folder->folderName();
        //Remove the real folder.
        //! FIXME: Add codes here.
        //Clear the folder.
        folder->deleteLater();
    }
    //Update the folder list.
    mailAccount->setCustomFolders(updatedCustomFolders);
    //Get the response.
    return true;
}

bool KNMailImapProtocol::waitAndCheckResponse(QList<QByteArray> *responseCache)
{
    //We have to try several times.
    int tries=5;
    //Do tries times loop.
    while(tries--)
    {
        //Wait for response.
        QList<QByteArray> imapResponse;
        if(waitForResponse(&imapResponse))
        {
            //Check the response.
            QString lastResponse=imapResponse.last();
            //We got OK from the server, that means mission complete.
            if(lastResponse.startsWith(m_header+" OK"))
            {
                //Check the response cache.
                if(responseCache)
                {
                    //Save the string list.
                    (*responseCache)=imapResponse;
                }
                return true;
            }
            //Or else, login data error.
            return false;
        }
    }
    //We have tried it for several times, but it cannot get the data.
    return false;
}
