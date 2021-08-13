#ifndef TABDIALOG_H
#define TABDIALOG_H

#include "helmos-andor2k/cpp_socket.hpp"
#include <QDialog>
#include <QDialogButtonBox>
#include <QTabWidget>
#include <QWidget>

class TabDialog : public QDialog {
  Q_OBJECT
public:
  explicit TabDialog(andor2k::ClientSocket *&sock_ptr,
                     QWidget *parent = nullptr);

private:
  QTabWidget *m_tabWidget;
  QDialogButtonBox *m_buttonBox;
  char buffer[1024]; // todo delete this!
  andor2k::ClientSocket **csock;
};

#endif // TABDIALOG_H
