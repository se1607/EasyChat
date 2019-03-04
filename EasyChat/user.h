#ifndef USER_H
#define USER_H
#include <QObject>
#include <QQmlListProperty>
#include <QDebug>
class User: public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ name WRITE setName)
    Q_PROPERTY(QString password READ password WRITE setPassword)
    Q_PROPERTY(QQmlListProperty<User> friends READ friends)
public:
    User(QObject *parent = 0);

    QString name() const {
        return m_name;
    }
    QString password() const {
        return m_password;
    }

    void setName(const QString& n) {
        m_name = n;
    }
    void setPassword(const QString& p) {
        m_password = p;
    }

    QQmlListProperty<User> friends();

    void appendFriend(User* u);
    void clearFriend(User* u);
public slots:
    void cppSlot(const QString &msg)
    {
        qDebug() << msg;
    }
private:
    QString m_name;
    QString m_password;
    QList<User *> m_friends;
};

#endif // USER_H
