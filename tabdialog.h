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
  explicit TabDialog(QWidget *parent = nullptr);

private:
  QTabWidget *m_tabWidget;
  QDialogButtonBox *m_buttonBox;
  andor2k::ClientSocket *csock = nullptr;
};

#endif // TABDIALOG_H
