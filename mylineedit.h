#ifndef MYLINEEDIT_H
#define MYLINEEDIT_H

#include <QLineEdit>
#include <QWidget>
#include <QMouseEvent>

class MyLineEdit : public QLineEdit
{
  Q_OBJECT

public:
  MyLineEdit(QWidget *parent = 0);
  ~MyLineEdit();

signals:
  void clicked(MyLineEdit *line);

protected:
  virtual void mousePressEvent(QMouseEvent *e);
};

#endif // MYLINEEDIT_H
