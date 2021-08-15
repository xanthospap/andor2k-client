#include "advancedsettings.h"
#include "helmos-andor2k/andor2k.hpp"
#include <QGridLayout>
#include <QGroupBox>
#include <QLabel>
#include <QMessageBox>
#include <QRegularExpression>
#include <QRegularExpressionValidator>
#include <QStringList>
#include <cstdio>
#include <cstring>

using andor2k::ClientSocket;
using andor2k::Socket;

int AdvancedSettings::make_command(char *buffer) {

  auto smode = m_acquisition_mode->currentText();
  int imode;
  if (smode == "Run Till Abort") {
    imode = AcquisitionMode2int(AcquisitionMode::RunTillAbort);
  } else if (smode == "Kinetic Series") {
    imode = AcquisitionMode2int(AcquisitionMode::KineticSeries);
  } else {
    QMessageBox msbox(QMessageBox::Critical, "Parameter Error",
                      "Failed to translate acquisition mode to int");
    msbox.exec();
    return 1;
  }

  int sz = 0;
  std::memset(buffer, 0, MAX_SOCKET_BUFFER_SIZE);
  sz = std::sprintf(buffer, "setparam acqmode=%d", imode);

  if (smode == "Kinetic Series") {
    bool ok;
    float kct = m_kinetic_ct->text().toFloat(&ok);
    if (!ok) {
      QMessageBox msbox(QMessageBox::Critical, "Parameter Error",
                        "Failed to translate kinetic cycle time to float");
      msbox.exec();
      return 1;
    }
    sz += std::sprintf(buffer + sz, " kineticcycletime=%.3f", kct);
  }

  return 0;
}

void AdvancedSettings::send_command() {
  // check that the connection/socket is alive
  if (*csock == nullptr) {
    QMessageBox msbox(QMessageBox::Critical, "Connection Error",
                      "Cannot send command to daemon! Need to connect first");
    msbox.exec();
    return;
  }

  if (this->make_command(this->buffer))
    return;

    // send command to deamon
#ifdef DEBUG
  printf("[DEBUG][ANDOR2K::client::%15s] sending command: \"%s\"\n", __func__,
         buffer);
#endif
  (*csock)->send(buffer);
  printf("--> parameter command: [%s] <--\n", buffer);

  return;
}

AdvancedSettings::AdvancedSettings(ClientSocket *&csocket, QWidget *parent)
    : QWidget(parent) {
  createGui();
  setLayout(main_layout);
  csock = &csocket;

  connect(m_set_btn, SIGNAL(clicked()), this, SLOT(send_command()));

  connect(m_acquisition_mode,
          QOverload<int>::of(&QComboBox::currentIndexChanged), [=](int index) {
            if (index == 1)
              this->m_kinetic_ct->setEnabled(true);
            else
              this->m_kinetic_ct->setEnabled(false);
          });
}

void AdvancedSettings::createGui() {
  // Options for acquisition mode
  QStringList types = {"Run Till Abort", "Kinetic Series"};
  m_acquisition_mode = new QComboBox(this);
  m_acquisition_mode->addItems(types);

  m_kinetic_ct = new QLineEdit;
  QRegularExpression rx_ex("[0-9.]{1,10}");
  QValidator *validator_ex = new QRegularExpressionValidator(rx_ex, this);
  m_kinetic_ct->setValidator(validator_ex);
  m_kinetic_ct->setToolTip("Kinetic cycle time in seconds, as float value.");
  m_kinetic_ct->setEnabled(false);

  QGroupBox *acquisition_gbox = new QGroupBox(tr("Acquisition Mode"));
  QGridLayout *acquisition_layout = new QGridLayout;
  acquisition_layout->addWidget(
      new QLabel(tr("Acquisition Mode for Multiple Scans")), 0, 0);
  acquisition_layout->addWidget(m_acquisition_mode, 0, 1);
  acquisition_layout->addWidget(new QLabel(tr("Kinetic Cycle Time (sec)")), 1,
                                0);
  acquisition_layout->addWidget(m_kinetic_ct, 1, 1);
  acquisition_gbox->setLayout(acquisition_layout);

  m_set_btn = new QPushButton("Set", this);

  main_layout = new QVBoxLayout;
  main_layout->addWidget(acquisition_gbox);
  main_layout->addWidget(m_set_btn);

  return;
}
