#ifndef KMMAILSENDERMANAGER_H
#define KMMAILSENDERMANAGER_H

#include <QObject>
#include <QList>

class KNMailAccount;
class KMMimePart;
/*!
 * \brief The kmmailsendermanager class
 */

#define kmSenderManager (KMMailSenderManager::instance())

class KMMailSenderManager : public QObject
{
    Q_OBJECT
public:
    static KMMailSenderManager *instance();

    static void initial(QObject *parent);

signals:
    void requireSend();

public slots:
    void appendMailSendList(KNMailAccount *account, KMMimePart *content);

private slots:
    void onSendNext();

private:
    struct sendPack
    {
        KNMailAccount *account;
        KMMimePart *content;
    };

    QList<sendPack> m_sendList;
    explicit KMMailSenderManager(QObject *parent = 0);
    static KMMailSenderManager *m_instance;

};

#endif // KMMAILSENDERMANAGER_H
