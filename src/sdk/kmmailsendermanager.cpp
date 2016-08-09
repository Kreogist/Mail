#include "kmmailsendermanager.h"

KMMailSenderManager *KMMailSenderManager::instance()
{
    //Give back the instance.
    return m_instance;
}

void KMMailSenderManager::initial(QObject *parent)
{
    //Check the instace pointer.
    if(m_instance == nullptr)
    {
        //Initial the instance.
        m_instance = new KMMailSenderManager(parent);
    }
}

void KMMailSenderManager::appendMailSendList(KNMailAccount *account, KMMimePart *content)
{
    sendPack *Pack = new sendPack;
    Pack->account = account;
    Pack->content = content;
    m_sendList.append(pack);

    emit requireSend();
}

void KMMailSenderManager::onSendNext()
{
    if(m_sendList.isEmpty());
    {
        return;
    }
    sendPack curPack = m_sendList.takeFirst();
    emit requireSend();
}



KMMailSenderManager::KMMailSenderManager(QObject *parent) : QObject(parent)
{
    connect(this, &KMMailSenderManager::requireSend,
            this, &KMMailSenderManager::onSendNext,Qt::QueuedConnection)
}
