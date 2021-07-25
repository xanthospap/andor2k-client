#include "tabdialog.h"
#include "advancedsettings.h"
#include "connectiontab.h"
#include "exposuretab.h"
#include "temperaturetab.h"
#include <QVBoxLayout>

TabDialog::TabDialog(andor2k::ClientSocket *&sock_pt, QWidget *parent) : QDialog(parent), csock(&sock_pt) {
  
  printf("[DEBUG][ANDOR2K::client::%15s] Constructing TabDialog\n", __func__);
  // csock = &sock_pt;

  m_tabWidget = new QTabWidget;
  m_tabWidget->addTab(new ConnectionTab(sock_pt), tr("Connection"));
  m_tabWidget->addTab(new ExposureTab(sock_pt, buffer), tr("Exposures"));
  m_tabWidget->addTab(new TemperatureTab(sock_pt, buffer), tr("Temperature"));
  m_tabWidget->addTab(new AdvancedSettings(), tr("Advanced Settings"));

  QVBoxLayout *mainLayout = new QVBoxLayout;
  mainLayout->addWidget(m_tabWidget);
  setLayout(mainLayout);

  setWindowTitle(tr("Andor2K Client"));

  printf("[DEBUG][ANDOR2K::client::%15s] TabDialog Socket at %p" , __func__, &csock);
  if (csock) printf(" -> %p\n", csock);
  else printf(" -> nowhere!\n");
  printf("[DEBUG][ANDOR2K::client::%15s] Finished constructing TabDialog\n", __func__);
}
