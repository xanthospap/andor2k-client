#include "tabdialog.h"
#include "advancedsettings.h"
#include "connectiontab.h"
#include "exposuretab.h"
#include "temperaturetab.h"
#include <QVBoxLayout>

TabDialog::TabDialog(QWidget *parent) : QDialog(parent) {
  andor2k::ClientSocket *csock = nullptr;

  m_tabWidget = new QTabWidget;
  m_tabWidget->addTab(new ConnectionTab(csock), tr("Connection"));
  m_tabWidget->addTab(new ExposureTab(), tr("Exposures"));
  m_tabWidget->addTab(new TemperatureTab(), tr("Temperature"));
  m_tabWidget->addTab(new AdvancedSettings(), tr("Advanced Settings"));

  QVBoxLayout *mainLayout = new QVBoxLayout;
  mainLayout->addWidget(m_tabWidget);
  setLayout(mainLayout);

  setWindowTitle(tr("Andor2K Client"));
}
