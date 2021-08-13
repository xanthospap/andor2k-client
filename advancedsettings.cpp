#include "advancedsettings.h"
#include "andor2k.hpp"
#include <QStringList>
#include <QGroupBox>
#include <QGridLayout>
#include <QLabel>
#include <QMessageBox>
#include <cstdio>

using andor2k::ClientSocket;
using andor2k::Socket;

int AdvancedSettings::make_acquisition_command() {

  auto smode = m_acquisition_mode->currentText();
  int imode;
  if (smode == "Run Till Abort") {
    imode = AcquisitionMode2int(AcquisitionMode::RunTillAbort);
  } else if (smode == "Kinetic Series") {
    imode = AcquisitionMode2int(AcquisitionMode::KineticSeries);
  } else {
    QMessageBox msbox(QMessageBox::Critical, "Acquisition Mode Error", "Failed to translate acquisition mode to int");
    msbox.exec();
    return 1;
  }

  std::memset(buffer, 0, MAX_SOCKET_BUFFER_SIZE);
  std::sprintf(buffer, "setparam acqmode=%d", imode);
  return 0; 
}

AdvancedSettings::AdvancedSettings(ClientSocket *&csocket, QWidget *parent) 
  : QWidget(parent) 
{
  createGui();
  setLayout(main_layout);
  csock = &csocket;
}

void AdvancedSettings::createGui() { 
  // Options for acquisition mode
  QStringList types = {"Run Till Abort", "Kinetic Series"};
  m_acquisition_mode = new QComboBox(this);
  m_acquisition_mode->addItems(types);
  
  QGroupBox *acquisition_gbox = new QGroupBox(tr("Acquisition Mode"));
  QGridLayout *acquisition_layout = new QGridLayout;
  acquisition_layout->addWidget(new QLabel(tr("Acquisition Mode for Multiple Scans")), 0, 0);
  acquisition_layout->addWidget(m_acquisition_mode, 0, 1);
  acquisition_gbox->setLayout(acquisition_layout);

  m_set_btn = new QPushButton("Set", this);
  
  main_layout = new QVBoxLayout;
  main_layout->addWidget(acquisition_gbox);
  main_layout->addWidget(m_set_btn);
  
  return; 
}
