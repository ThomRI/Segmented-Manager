#ifndef COMMANDLISTENER_H
#define COMMANDLISTENER_H


class CommandListener : public QObject
{
    Q_OBJECT

    public:
        CommandListener();
        virtual ~CommandListener();
    public slots:
        void dataReceived(QDataStream *stream);
    protected:
    private:

        DemoManager *m_demoManager;

};

#endif // COMMANDLISTENER_H
