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


private:
    QString m_name;
    QString m_password;
};

#endif // USER_H
