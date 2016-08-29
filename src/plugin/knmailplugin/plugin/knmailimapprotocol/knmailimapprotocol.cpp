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
#include <QFile>

#include "mime/knmimepart.h"
#include "mime/knmimeparser.h"
#include "knmailmodel.h"
#include "knmailaccount.h"
#include "knmailutil.h"
#include "knutil.h"

#include "knmailimapprotocol.h"

#include <QDebug>

#define TriedTimes 5

KNMailImapProtocol::KNMailImapProtocol(QObject *parent) :
    KNMailReceiverProtocol(parent),
    m_header(QString("*")),
    m_idCode(0),
    m_loginState(false)
{
}

bool KNMailImapProtocol::connectToHost()
{
    qDebug()<<"Start to connect.";
    //Get the receive config.
    const KNMailProtocolConfig &config=account()->receiveConfig();
    //Connect to host server.
    if(!startConnection(config))
    {
        //Can't connect to server.
        setLastError(ConnectionFailed);
        qDebug()<<"Connected failed.";
        return false;
    }
    qDebug()<<"Wait for connected response.";
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
    if(socket() && socket()->state()!=QAbstractSocket::ConnectedState &&
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
    //Get the config.
    const KNMailProtocolConfig &config=mailAccount->receiveConfig();
    //Send login command.
    qDebug()<<"Start to login.";
    sendImapMessage("LOGIN "+ config.loginFormat.arg(mailAccount->username(),
                                                     mailAccount->password()));
    //Wait for response.
    if(!waitAndCheckResponse())
    {
        //Username and password is not correct.
        setLastError(LoginPasswordError);
        //Failed to login.
        return false;
    }
    qDebug()<<"Login success.";
    if(config.idCheck)
    {
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
    }
    //Set the login state.
    m_loginState=true;
    //Login success.
    return true;
}

bool KNMailImapProtocol::updateFolderStatus()
{
    qDebug()<<"Start to login.";
    //Check the login state.
    if((!m_loginState) && (!login()))
    {
        //Failed to login, then failed to update the status.
        return false;
    }
    qDebug()<<"Login complete.";
    //Prepare the cache.
    QList<QByteArray> folderStatus;
    qDebug()<<"Start to list folder.";
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
    //Get the account directory path.
    QString accountDirectory=mailAccount->accountDirectoryPath();
    //Check the folder status.
    for(auto i : folderStatus)
    {
        //Translate all the data.
        QString folderFlag,
                folderName=findFolderName(KNMailUtil::fromUtf7(i), &folderFlag);
        //Check the folder info.
        if(folderName.isEmpty())
        {
            continue;
        }
        //Get the raw folder name.
        QString rawFolderName=findFolderName(i);
        //Remove the possible quote.
        if(folderName.size()>2 && folderName.at(0)=='\"')
        {
            //Remove the quote on both side.
            folderName=folderName.mid(1, folderName.size()-2);
            rawFolderName=rawFolderName.mid(1, rawFolderName.size()-2);
        }
        qDebug()<<folderFlag<<folderName<<rawFolderName;
        //Define the current model.
        KNMailModel *currentModel;
        //Check whether the folder name is exist in the name list.
        if(folderName=="INBOX")
        {
            currentModel=mailAccount->folder(FolderInbox);
            //Update the inbox name of the folder.
            currentModel->setServerName("INBOX");
            //Update the folder..
            updateFolder(currentModel);
            //Goto Next.
            continue;
        }
        if(folderFlag.contains("\\Sent"))
        {
            //System folder, Sent items.
            currentModel=mailAccount->folder(FolderSentItems);
            currentModel->setServerName(rawFolderName);
            //Update the folder..
            updateFolder(currentModel);
            //Goto next.
            continue;
        }
        if(folderFlag.contains("\\Junk"))
        {
            //System folder, Junk mail.
            currentModel=mailAccount->folder(FolderJunk);
            currentModel->setServerName(rawFolderName);
            //Update the folder..
            updateFolder(currentModel);
            //Goto next.
            continue;
        }
        if(folderFlag.contains("\\Trash"))
        {
            //System folder, Trash can.
            currentModel=mailAccount->folder(FolderTrash);
            currentModel->setServerName(rawFolderName);
            //Update the folder..
            updateFolder(currentModel);
            //Goto next.
            continue;
        }
        if(folderFlag.contains("\\Drafts"))
        {
            //System folder, drafts.
            currentModel=mailAccount->folder(FolderDrafts);
            currentModel->setServerName(rawFolderName);
            //Update the folder.
            updateFolder(currentModel);
            //Goto next.
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
                currentModel=customFolders.takeAt(i);
                //Update current model information.
                currentModel->setServerName(rawFolderName);
                currentModel->setManagedAccount(mailAccount);
                //Add folder to folder list.
                updatedCustomFolders.append(currentModel);
                //Update the folder.
                updateFolder(currentModel);
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
        //We didn't find the folder, we have to construct this folder.
        currentModel=new KNMailModel();
        //Set the folder name.
        currentModel->setFolderName(folderName);
        currentModel->setServerName(rawFolderName);
        currentModel->setManagedAccount(mailAccount);
        //Save the content to create the directory.
        currentModel->saveToFolder(accountDirectory);
        //Add folder to updated list.
        updatedCustomFolders.append(currentModel);
        //Update the folder data.
        updateFolder(currentModel);
    }
    //Set the folders.
    emit requireUpdateFolders(updatedCustomFolders);
    //Clear all the left folder in the original list.
    while(!customFolders.isEmpty())
    {
        //Get one folder.
        KNMailModel *folder=customFolders.takeLast();
        //Remove the real folder.
        folder->removeModelContent(accountDirectory);
        //Clear the folder.
        folder->deleteLater();
    }
    //Get the response.
    return true;
}

bool KNMailImapProtocol::updateFolder(KNMailModel *folder)
{
    //Check the login state.
    if((!m_loginState) && (!login()))
    {
        //Failed to login, then failed to update the status.
        return false;
    }
    //Select the folder first.
    sendImapMessage("SELECT \"" + folder->serverName() + "\"");
    //Wait and check response.
    if(!waitAndCheckResponse())
    {
        qDebug()<<"No response from server.";
        //Reset the login state, because the connection is cut.
        m_loginState=false;
        //Failed to select the folder.
        return false;
    }
    //Prepare the cache.
    QList<QByteArray> *folderStatus=new QList<QByteArray>();
    //Get the current uid content list.
    sendImapMessage("FETCH 1:* UID");
    //Get the response.
    if(!waitAndCheckResponse(folderStatus))
    {
        //Recover the memory.
        delete folderStatus;
        //Failed to fetch mail information.
        return false;
    }
    //Create a list for the content.
    QList<int> *uidList=new QList<int>();
    //Check all the response.
    for(auto i : (*folderStatus))
    {
        //Check the content of i.
        if(i.isEmpty())
        {
            //Move to next line.
            continue;
        }
        //Check the first char.
        if(i.at(0)=='*')
        {
            //The line we want, check the UID position.
            int uidPos=i.indexOf("UID");
            //Check the uid position validation.
            if(uidPos==-1)
            {
                //Move on to the next line.
                continue;
            }
            //Skip the UID letters.
            uidPos+=3;
            //Get the back parenthese position.
            int parenthesePos=i.indexOf(')', uidPos);
            //Check the position.
            if(parenthesePos==-1)
            {
                //Move on.
                continue;
            }
            //Get the uid.
            uidList->append((i.mid(uidPos, parenthesePos-uidPos)).toInt());
        }
    }
    //Recover the memory.
    delete folderStatus;
    //The uid should be sort from small to big, reverse the list.
    std::reverse(uidList->begin(), uidList->end());
    //Update the folder uid list.
    folder->updateUidList(account()->accountDirectoryPath(), uidList);
    //Recover the list memory.
    delete uidList;
    //Update the data successfully.
    return true;
}

bool KNMailImapProtocol::updateFolderContent(KNMailModel *folder,
                                             int startPosition,
                                             int endPosition)
{
    //Check the login state.
    if((!m_loginState) && (!login()))
    {
        //Failed to login, then failed to update the status.
        return false;
    }
    //Select the folder first.
    sendImapMessage("SELECT \"" + folder->serverName() + "\"");
    //Wait and check response.
    if(!waitAndCheckResponse())
    {
        qDebug()<<"No response from server.";
        //Reset the login state, because the connection is cut.
        m_loginState=false;
        //Failed to select the folder.
        return false;
    }
    qDebug()<<"Begin download E-mail from"<<startPosition<<"to"<<endPosition;
    //Loop for all the content.
    for(int i=startPosition; i<endPosition; ++i)
    {
        //Check whether we need to update the data.
        if(folder->isItemCached(i))
        {
            qDebug()<<"Cached!";
            //Ignore the item which is already cached.
            continue;
        }
        //Save the index.
        QString mailServerIndex=QString::number(folder->rowCount()-i),
                mailFileDir=account()->accountDirectoryPath() + "/" +
                folder->folderName(),
                mailFilePath= mailFileDir + "/" +
                QString::number(folder->uid(i)) + ".eml";
        //Ensure the folder is existed.
        if(KNUtil::ensurePathValid(mailFileDir).isEmpty())
        {
            //Ignore this mail.
            continue;
        }
        //Open the file.
        QFile emlFile(mailFilePath);
        //Open the file as write only mode.
        if(!emlFile.open(QIODevice::WriteOnly))
        {
            qDebug()<<"Cannot open file"<<mailFilePath;
            //Ignore the current file.
            continue;
        }
        qDebug()<<"Start fetching"<<mailServerIndex<<"header";
        //Fetch all the E-mail content down.
        sendImapMessage("FETCH " + mailServerIndex + " body[HEADER]");
        //Wait for response from server.
        int tries=TriedTimes;
        while(tries--)
        {
            //Wait for response from server.
            if(!waitForReadyRead())
            {
                //Close the cache file.
                emlFile.close();
                //Move to next try.
                continue;
            }
            //Response data cache.
            QByteArray responseData;
            //Reset the tries.
            tries=TriedTimes;
            //Try to read all content from the socket.
            while(socket()->bytesAvailable() > 0)
            {
                //Generate the mail file.
                responseData=socket()->readLine();
                //Check the response data.
                if(responseData.startsWith("*") ||
                        responseData.startsWith(" FLAGS") ||
                        responseData.startsWith(m_header.toLatin1() + " OK"))
                {
                    //This line should be ignored, the content of current line
                    //shows:
                    // * UID FETCH (BODY[HEADER] {size}
                    // or
                    // AXX OK FETCH completed.
                    continue;
                }
                //Write the content to the cache file.
                emlFile.write(responseData);
                qDebug()<<responseData;
            }
            //Check the data is still left or not.
            if(socket()->bytesAvailable()==0)
            {
                //Stop looping.
                break;
            }
        }
        //Check the tries time.
        if(tries==0)
        {
            //The header is not read complete.
            //Close the cache file.
            emlFile.close();
            //Move to next mail.
            continue;
        }
        qDebug()<<"Start fetching"<<mailServerIndex<<"content";
        //Fetch all the E-mail body data down.
        sendImapMessage("FETCH " + mailServerIndex + " body[TEXT]");
        //Wait for response from server.
        tries=TriedTimes;
        while(tries--)
        {
            //Wait for response from server.
            if(!waitForReadyRead())
            {
                //Close the cache file.
                emlFile.close();
                //Move to next try.
                continue;
            }
            //Response data cache.
            QByteArray responseData;
            //Reset the tries.
            tries=TriedTimes;
            //Try to read all content from the socket.
            while(socket()->bytesAvailable() > 0)
            {
                //Generate the mail file.
                responseData=socket()->readLine();
                //Check the response data.
                if(responseData.startsWith("*") ||
                        responseData.startsWith(m_header.toLatin1() + " OK") ||
                        responseData.startsWith(" FLAGS"))
                {
                    //This line should be ignored, the content of current line
                    //shows:
                    // * UID FETCH (BODY[TEXT] {size}
                    // or
                    //  FLAGS (\Seen))
                    // or
                    // AXX OK FETCH completed.
                    continue;
                }
                //Write the content to the cache file.
                emlFile.write(responseData);
            }
            //Check the data is still left or not.
            if(socket()->bytesAvailable()==0)
            {
                //Stop looping.
                break;
            }
        }
        //Close the cache file.
        emlFile.close();
        qDebug()<<i<<"fetch complete.";
        //Parse the mail content.
        KNMimePart *parsedMail=KNMimeParser::parseMime(mailFilePath);
        //Update the item information of the model.
        KNMailListItem cachedItem=folder->item(i);
        //Update the information of the item.
        cachedItem.cached=true;
        cachedItem.title=KNMailUtil::parseEncoding(
                    parsedMail->mimeHeader("subject").simplified());
        cachedItem.sender=KNMailUtil::parseMailAddress(
                    parsedMail->mimeHeader("from"),
                    cachedItem.senderName);
        //Recover the parse memory.
        delete parsedMail;
        //Update the item.
        folder->updateItem(i, cachedItem);
        //Save the folder content.
        folder->saveToFolder(folder->managedAccount()->accountDirectoryPath());
    }
    //Mission complete.
    return true;
}

void KNMailImapProtocol::updateProtocolConfig()
{
    //Do original configure.
    KNMailReceiverProtocol::updateProtocolConfig();
    //Link with the account.
    connect(this, &KNMailImapProtocol::requireUpdateFolders,
            account(), &KNMailAccount::setCustomFolders,
            Qt::QueuedConnection);
}

QString KNMailImapProtocol::findFolderName(const QString &rawFolderInfo,
                                           QString *folderFlag)
{
    //Parse the folder info.
    if(!rawFolderInfo.startsWith("* LIST "))
    {
        //This means that this line doesn't contains information about the
        //folder.
        return QString();
    }
    //Remove the LIST.
    QString folderInfo=rawFolderInfo.mid(7);
    //Find the flag part and name parts.
    int flagEndPosition=folderInfo.indexOf(')');
    if(flagEndPosition==-1)
    {
        //This line cannot be used.
        return QString();
    }
    //Find the folder position.
    int quoteStart=folderInfo.indexOf('\"', flagEndPosition);
    //If can't find quote, end.
    if(quoteStart==-1)
    {
        return QString();
    }
    //Find quote end.
    int quoteEnd=folderInfo.indexOf('\"', quoteStart+1);
    //If can't find end quote, end.
    if(quoteEnd==-1)
    {
        return QString();
    }
    //Check directory path, if it is not "\", ignore the folder.
    if(folderInfo.mid(quoteStart, quoteEnd-quoteStart+1)!="\"/\"")
    {
        //Ignore the folder which is not at the root directory.
        return QString();
    }
    //Get the flag part and name part.
    if(folderFlag)
    {
        //Update the folder flag.
        (*folderFlag)=folderInfo.left(flagEndPosition+1);
    }
    //Get the folder name.
    return folderInfo.mid(quoteEnd+1).simplified();
}

bool KNMailImapProtocol::waitAndCheckResponse(QList<QByteArray> *responseCache)
{
    //We have to try several times.
    int tries=TriedTimes;
    //Do tries times loop.
    while(tries--)
    {
        //Wait for response.
        QList<QByteArray> imapResponse;
        if(waitForResponse(&imapResponse))
        {
            //Reset the tries time.
            tries=TriedTimes;
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
    //Disconnet from the server.
    socket()->disconnectFromHost();
    //Reset the login state, because the connection is cut.
    m_loginState=false;
    //We have tried it for several times, but it cannot get the data.
    return false;
}
