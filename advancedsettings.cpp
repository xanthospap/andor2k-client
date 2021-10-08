#include "advancedsettings.h"
#include "cliutils.h"
// #include "settempthread.h"
#include "imagethread.h"
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
#include <chrono>
#ifdef DEBUG
#include <iostream>
#endif

using andor2k::ClientSocket;
using andor2k::Socket;

int AdvancedSettings::make_temp_command(char *buffer) {
  bool ok;
  int target_temp = m_set_temp->text().toInt(&ok);
  if (!ok) {
    QMessageBox msbox(QMessageBox::Critical, "Parameter Error",
                      "Failed to translate target temperature to int");
    msbox.exec();
    return 1;
  }

  if (target_temp<ANDOR_MIN_TEMP || target_temp>ANDOR_MAX_TEMP) {
    QMessageBox msbox(QMessageBox::Critical, "Parameter Error",
                      "Invalid temperature value! Temperature out of valid range");
    msbox.exec();
    return 1;
  }

  std::memset(buffer, 0, MAX_SOCKET_BUFFER_SIZE);
  return (!std::sprintf(buffer, "settemp %d", target_temp));
}

int AdvancedSettings::make_acquisition_command(char *buffer) {

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

int AdvancedSettings::check_socket() {
  // check that the connection/socket is alive
  if (*csock == nullptr) {
    QMessageBox msbox(QMessageBox::Critical, "Connection Error",
                      "Cannot send command to daemon! Need to connect first");
    msbox.exec();
    return 1;
  }
  return 0;
}

int AdvancedSettings::send_command(const  char* command) {
  // send command to deamon
#ifdef DEBUG
  printf("[DEBUG][ANDOR2K::client::%15s] sending command: \"%s\"\n", __func__,
         command);
#endif
  printf("--> parameter command: [%s] <--\n", command);
  return (*csock)->send(command);
}

void AdvancedSettings::send_acquisition_command() {
  // is the socket ok?
  if (check_socket()) return;

  // write the command to be sent to the buffer
  char buffer[MAX_SOCKET_BUFFER_SIZE];
  if (make_acquisition_command(buffer)) return;

  // send command to deamon
  /*int sent_bts = */send_command(buffer);

  return;
}

void AdvancedSettings::send_temperature_command() {
  // is the socket ok?
  if (check_socket()) return;

  // write the command to be sent to the buffer
  char buffer[MAX_SOCKET_BUFFER_SIZE];
  if (make_temp_command(buffer)) return;

  // enable edits
  // enable_temperature_edits(true);

  // send command to deamon
  int sent_bts = send_command(buffer);
  if (sent_bts < 1) return;
  
  // starting timing
  start_set_temp = std::chrono::high_resolution_clock::now();
  
  // start listening thread for server info
  ImageThread *ithread = new ImageThread(*csock, this);

  connect(ithread, &ImageThread::resultReady, this,
          &AdvancedSettings::serverJobDone);
  connect(ithread, &ImageThread::newResponseReady, this,
          &AdvancedSettings::serverJobUpdate);
  connect(ithread, &ImageThread::finished, ithread, &QObject::deleteLater);

  ithread->start();

  return;

}

void AdvancedSettings::serverJobDone(const QString &response) {
  serverJobUpdate(response);
  QStringList list(split_command(response));

  int ierror=0;
  if (list_signals_done(list, &ierror)) {
    if (!ierror) {
      QMessageBox msbox(QMessageBox::Information, "Temperature Status",
          "All done! Temperature set and stabilized!");
      msbox.exec();
    } else {
      QString info("Failed to set target temperature! (status:");
      info += get_val("error", list);
      info += ")\n";
      info += get_val("status", list);
      QMessageBox msbox(QMessageBox::Critical, "Temperature Status", info);
      msbox.exec();
    }
  }
  //enable_temperature_edits(false);
  return;
}

void AdvancedSettings::serverJobUpdate(const QString &response) {
#ifdef DEBUG
  printf("--> got string from server: [%s] <---\n",
         response.toStdString().c_str());
#endif
  QStringList list(split_command(response));
  bool converted;

  // temperature
  // int ctemp = get_val("ctemp", list).toInt(&converted);
  //if (converted) {
  m_ctemp->setText(get_val("ctemp", list));
  // printf("\t>> setting ctemp QLineEdit to %s\n", QString::number(ctemp).toStdString().c_str());
  //}
  
  // status
  m_temp_status->setText(get_val("status", list));
  printf("\t>> setting temp_status QLineEdit to %s\n", get_val("status", list).toStdString().c_str());
  
  // elapsed time
  long et = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::high_resolution_clock::now() - start_set_temp).count();
  if (et<120) {
    m_temp_et->setText(QString::number(et) + QString(" sec. / ") + get_val("time", list));
  } else {
    m_temp_et->setText(QString::number(et/60) + QString(" min. / ") + get_val("time", list));
  }
  
  return;
}

AdvancedSettings::AdvancedSettings(ClientSocket *&csocket, QWidget *parent)
    : QWidget(parent) {
  createGui();
  setLayout(main_layout);
  csock = &csocket;

  connect(m_set_btn, SIGNAL(clicked()), this, SLOT(send_acquisition_command()));
  connect(m_settmp_btn, SIGNAL(clicked()), this, SLOT(send_temperature_command()));

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
  
  m_set_temp = new QLineEdit;
  QRegularExpression rx_tp("-?[0-9]{1,10}");
  QValidator *validator_tp = new QRegularExpressionValidator(rx_tp, this);
  m_set_temp->setValidator(validator_tp);
  m_set_temp->setToolTip("Set target temperature for camera (in Celsius).");
  m_set_temp->setEnabled(true);
  m_ctemp = new QLineEdit;
  //m_ctemp->setEnabled(false);
  m_temp_status = new QLineEdit;
  //m_temp_status->setEnabled(false);
  m_temp_et = new QLineEdit;
  //m_temp_et->setEnabled(false);

  QGroupBox *temp_gbox = new QGroupBox(tr("Camera Temperature"));
  QGridLayout *temp_layout = new QGridLayout;
  temp_layout->addWidget(new QLabel(tr("Set Target Tmperature (C)")), 0,0,1,1);
  temp_layout->addWidget(m_set_temp, 0,1,1,1);
  temp_layout->addWidget(new QLabel(tr("Current Temperature")),0,2,1,1);
  temp_layout->addWidget(m_ctemp, 0,3,1,1);
  temp_layout->addWidget(new QLabel(tr("Status")),1,0,1,1);
  temp_layout->addWidget(m_temp_status, 1,1,1,1);
  temp_layout->addWidget(new QLabel(tr("Elapsed Time (sec)")),1,2,1,1);
  temp_layout->addWidget(m_temp_et, 1,3,1,1);
  temp_gbox->setLayout(temp_layout);

  m_set_btn = new QPushButton("Set", this);
  m_settmp_btn = new QPushButton("Set", this);

  main_layout = new QVBoxLayout;
  main_layout->addWidget(acquisition_gbox);
  main_layout->addWidget(m_set_btn);
  main_layout->addWidget(temp_gbox);
  main_layout->addWidget(m_settmp_btn);

  return;
}

void AdvancedSettings::enable_temperature_edits(bool enable) {
  m_ctemp->setEnabled(enable);
  m_temp_status->setEnabled(enable);
  m_temp_et->setEnabled(enable);
}
