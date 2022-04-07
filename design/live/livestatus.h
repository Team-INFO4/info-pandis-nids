#ifndef LIVESTATUS_H
#define LIVESTATUS_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class Livestatus; }
QT_END_NAMESPACE

class Livestatus : public QMainWindow
{
    Q_OBJECT

public:
    Livestatus(QWidget *parent = nullptr);
    ~Livestatus();

private:
    Ui::Livestatus *ui;
};
#endif // LIVESTATUS_H
