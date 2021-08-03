#include "exposuretab.h"
#include <cstring>
#include <QApplication>
#include <QStringList>
#include <QGroupBox>
#include <QMessageBox>

using andor2k::ClientSocket;
using andor2k::Socket;

ExposureTab::ExposureTab(ClientSocket *&csocket, char* sock_buffer,  QWidget *parent)
    : QWidget(parent) {
  printf("[DEBUG][ANDOR2K::client::%15s] Constructing ExposureTab\n", __func__);
  createGui();
  setLayout(m_layout);
  csock = &csocket;
  buffer = sock_buffer;
  connect(m_start_button, SIGNAL(clicked()), this, SLOT(set_exposure()));
  
  printf("[DEBUG][ANDOR2K::client::%15s] ExposureTab Socket at %p -> %p", __func__, &csock, csock);
  if (*csock)
    printf(" -> %p\n", *csock);
  else
    printf(" -> nowhere!\n");
  printf("[DEBUG][ANDOR2K::client::%15s] Finished constructing ExposureTab\n", __func__);
}

void ExposureTab::set_exposure() {
  if ( this->make_command(this->buffer) ) return;
  /* send command to deamon */
  printf("[DEBUG][ANDOR2K::client::%15s] sending command: \"%s\"\n", __func__, buffer);
  (*csock)->send(buffer);
}

int ExposureTab::make_command(char *buffer) {
  std::size_t idx = 0;
  std::memset(buffer, '\0', 1024);
  std::strcpy(buffer+idx, "image ");
  idx = std::strlen(buffer);

  /* check for empty edits */
  char error_msg[128];
  std::memset(error_msg, 0, 128);
  int error = 0;
  if (m_filename_ledit->text().isEmpty()) {
    error = 1;
    std::strcpy(error_msg, "Image filename cannot be empty!");
  } else if (m_exposure_ledit->text().isEmpty()) {
    error = 1;
    std::strcpy(error_msg, "Exposure cannot be empty!");
  } else if (m_nimages_ledit->text().isEmpty()) {
    error = 1;
    std::strcpy(error_msg, "Number of Images cannot be empty!");
  } else if (m_vbin_ledit->text().isEmpty() || m_hbin_ledit->text().isEmpty()) {
    error = 1;
    std::strcpy(error_msg, "Vertical/Horizontal binning cannot be empty!");
  } else if (m_vstart_pix->text().isEmpty() || m_vend_pix->text().isEmpty()) {
    error = 1;
    std::strcpy(error_msg, "Vertical start/end pixels cannot be empty!");
  } else if (m_hstart_pix->text().isEmpty() || m_hend_pix->text().isEmpty()) {
    error = 1;
    std::strcpy(error_msg, "Horizontal start/end pixels cannot be empty!");
  }
  if (error) {
    QMessageBox msbox(QMessageBox::Critical, "Exposure Options Error", error_msg);
    msbox.exec();
    return error;
  }

  /* filename */
  std::strcpy(buffer+idx, "--filename ");
  idx = std::strlen(buffer);
  auto tval = m_filename_ledit->text().toStdString();
  std::strcpy(buffer+idx, tval.c_str());
  idx = std::strlen(buffer);
  buffer[idx] = ' ';
  ++idx;

  /* exposure */
  std::strcpy(buffer+idx, "--exposure ");
  idx = std::strlen(buffer);
  tval = m_exposure_ledit->text().toStdString();
  std::strcpy(buffer+idx, tval.c_str());
  idx = std::strlen(buffer);
  buffer[idx] = ' ';
  ++idx;

  /* nimages */
  std::strcpy(buffer+idx, "--nimages ");
  idx = std::strlen(buffer);
  tval = m_nimages_ledit->text().toStdString();
  std::strcpy(buffer+idx, tval.c_str());
  idx = std::strlen(buffer);
  buffer[idx] = ' ';
  ++idx;
  
  /* vbin */
  std::strcpy(buffer+idx, "--vbin ");
  idx = std::strlen(buffer);
  tval = m_vbin_ledit->text().toStdString();
  std::strcpy(buffer+idx, tval.c_str());
  idx = std::strlen(buffer);
  buffer[idx] = ' ';
  ++idx;
  
  /* hbin */
  std::strcpy(buffer+idx, "--hbin ");
  idx = std::strlen(buffer);
  tval = m_hbin_ledit->text().toStdString();
  std::strcpy(buffer+idx, tval.c_str());
  idx = std::strlen(buffer);
  buffer[idx] = ' ';
  ++idx;
  
  /* vstart */
  std::strcpy(buffer+idx, "--vstart ");
  idx = std::strlen(buffer);
  tval = m_vstart_pix->text().toStdString();
  std::strcpy(buffer+idx, tval.c_str());
  idx = std::strlen(buffer);
  buffer[idx] = ' ';
  ++idx;
  
  /* vend */
  std::strcpy(buffer+idx, "--vend ");
  idx = std::strlen(buffer);
  tval = m_vend_pix->text().toStdString();
  std::strcpy(buffer+idx, tval.c_str());
  idx = std::strlen(buffer);
  buffer[idx] = ' ';
  ++idx;
  
  /* hstart */
  std::strcpy(buffer+idx, "--hstart ");
  idx = std::strlen(buffer);
  tval = m_hstart_pix->text().toStdString();
  std::strcpy(buffer+idx, tval.c_str());
  idx = std::strlen(buffer);
  buffer[idx] = ' ';
  ++idx;
  
  /* hend */
  std::strcpy(buffer+idx, "--hend ");
  idx = std::strlen(buffer);
  tval = m_hend_pix->text().toStdString();
  std::strcpy(buffer+idx, tval.c_str());
  idx = std::strlen(buffer);
  buffer[idx] = ' ';
  ++idx;
  
  /* type */
  std::strcpy(buffer+idx, "--type ");
  idx = std::strlen(buffer);
  tval = m_type_cbox->currentText().toStdString();
  std::strcpy(buffer+idx, tval.c_str());
  idx = std::strlen(buffer);

  //printf("[DEBUG][ANDOR2K::client::%15s] Sending command to daemon: [%s]\n", __func__, buffer);
  printf("[DEBUG][ANDOR2K::client::%15s] Command size: %d\n", __func__, (int)std::strlen(buffer));
  return 0;
}

void ExposureTab::createGui() {
  m_filename_ledit = new QLineEdit;
  m_exposure_ledit = new QLineEdit;
  m_nimages_ledit = new QLineEdit;
  m_vbin_ledit = new QLineEdit;
  m_hbin_ledit = new QLineEdit;
  m_vstart_pix = new QLineEdit;
  m_hstart_pix = new QLineEdit;
  m_vend_pix = new QLineEdit;
  m_hend_pix = new QLineEdit;

  /* options for NumImages edit */
  QRegularExpression rx_ni("[0-9]{1,3}");
  QValidator *validator_ni = new QRegularExpressionValidator(rx_ni, this);
  m_nimages_ledit->setValidator(validator_ni);
  m_nimages_ledit->setToolTip("Number of images/exposures to be performed in run.");
  m_nimages_ledit->setText("1");
  
  /* options for Filename edit */
  QRegularExpression rx_fn(
      "[a-zA-Z0-9.]{1,127}"); /* only allow integers in range [-999, 999] */
  QValidator *validator_fn = new QRegularExpressionValidator(rx_fn, this);
  m_filename_ledit->setValidator(validator_fn);
  m_filename_ledit->setToolTip("Type in the filename of the exposure(s).\nDo not include path or the \".fits\" extension\nNote that the filename typed in here, will be augmented by the\ndate string (aka \"YYYYMMDD\" and a serial number starting from 1");
  m_filename_ledit->setText("img");
  
  /* options for Exposure edit */
  QRegularExpression rx_ex(
      "[0-9.]{1,10}"); /* only allow integers in range [-999, 999] */
  QValidator *validator_ex = new QRegularExpressionValidator(rx_ex, this);
  m_exposure_ledit->setValidator(validator_ex);
  m_exposure_ledit->setToolTip("Exposure time in seconds, as float value.");
  m_exposure_ledit->setText("0.5");
  
  /* options for Vertical Binning edit */
  QRegularExpression rx_vbn(
      "[0-9]{1,3}"); /* only allow integers in range [-999, 999] */
  QValidator *validator_vbn = new QRegularExpressionValidator(rx_vbn, this);
  m_vbin_ledit->setValidator(validator_vbn);
  m_vbin_ledit->setToolTip("Vertical binning, as integer value.");
  m_vbin_ledit->setText("1");
  
  /* options for Horizontal Binning edit */
  QRegularExpression rx_hbn(
      "[0-9]{1,3}"); /* only allow integers in range [-999, 999] */
  QValidator *validator_hbn = new QRegularExpressionValidator(rx_hbn, this);
  m_hbin_ledit->setValidator(validator_hbn);
  m_hbin_ledit->setToolTip("Horizontal binning, as integer value.");
  m_hbin_ledit->setText("1");
  
  /* options for Vertical Start Pixel edit */
  QRegularExpression rx_vstart(
      "[0-9]{1,4}"); /* only allow integers in range [-999, 999] */
  QValidator *validator_vstart = new QRegularExpressionValidator(rx_vstart, this);
  m_vstart_pix->setValidator(validator_vstart);
  m_vstart_pix->setToolTip("Vertical start pixel (inclusive); integer in range [1, 2048]");
  m_vstart_pix->setText("1");
  
  /* options for Vertical End Pixel edit */
  QRegularExpression rx_vend(
      "[0-9]{1,4}"); /* only allow integers in range [-999, 999] */
  QValidator *validator_vend = new QRegularExpressionValidator(rx_vend, this);
  m_vend_pix->setValidator(validator_vend);
  m_vend_pix->setToolTip("Vertical end pixel (inclusive); integer in range [1, 2048]");
  m_vend_pix->setText("2048");
  
  /* options for Horizontal Start Pixel edit */
  QRegularExpression rx_hstart(
      "[0-9]{1,4}"); /* only allow integers in range [-999, 999] */
  QValidator *validator_hstart = new QRegularExpressionValidator(rx_hstart, this);
  m_hstart_pix->setValidator(validator_hstart);
  m_hstart_pix->setToolTip("Horizontal start pixel (inclusive); integer in range [1, 2048]");
  m_hstart_pix->setText("1");
  
  /* options for Horizontal End Pixel edit */
  QRegularExpression rx_hend(
      "[0-9]{1,4}"); /* only allow integers in range [-999, 999] */
  QValidator *validator_hend = new QRegularExpressionValidator(rx_hend, this);
  m_hend_pix->setValidator(validator_hend);
  m_hend_pix->setToolTip("Horizontal end pixel (inclusive); integer in range [1, 2048]");
  m_hend_pix->setText("2048");

  /* Options for Type edit */
  QStringList types = {"flat", "object", "bias"};
  m_type_cbox = new QComboBox(this);
  m_type_cbox->addItems(types);
  
  m_label = new QLabel;
  m_label->setFrameStyle(QFrame::Box | QFrame::Plain);

  /* Buttons */
  m_cancel_button = new QPushButton("Cancel", this);
  m_start_button = new QPushButton("Start", this);
  m_start_button->setEnabled(true);
  m_cancel_button->setEnabled(false);

  /* group binning options */
  QGroupBox *bin_gbox = new QGroupBox(tr("Binning Options"));
  QGridLayout *bin_layout = new QGridLayout;
  bin_layout->addWidget(new QLabel(tr("Vertical")), 0, 0);
  bin_layout->addWidget(m_vbin_ledit, 0, 1);
  bin_layout->addWidget(new QLabel(tr("Horizontal")), 1, 0);
  bin_layout->addWidget(m_hbin_ledit, 1, 1);
  bin_gbox->setLayout(bin_layout);
  
  /* group image dimensions options */
  QGroupBox *pix_gbox = new QGroupBox(tr("Image Dimensions"));
  QGridLayout *pix_layout = new QGridLayout;
  pix_layout->addWidget(new QLabel(tr("Vertical")), 0, 1);
  pix_layout->addWidget(new QLabel(tr("Start")), 0, 2);
  pix_layout->addWidget(m_vstart_pix, 0, 3);
  pix_layout->addWidget(new QLabel(tr("End")), 0, 4);
  pix_layout->addWidget(m_vend_pix, 0, 5);
  pix_layout->addWidget(new QLabel(tr("Horizontal")), 1, 1);
  pix_layout->addWidget(new QLabel(tr("Start")), 1, 2);
  pix_layout->addWidget(m_hstart_pix, 1, 3);
  pix_layout->addWidget(new QLabel(tr("End")), 1, 4);
  pix_layout->addWidget(m_hend_pix, 1, 5);
  pix_gbox->setLayout(pix_layout);
  
  /* group general options */
  QGroupBox *gen_gbox = new QGroupBox(tr("General Options"));
  QGridLayout *gen_layout = new QGridLayout;
  gen_layout->addWidget(new QLabel(tr("Filename:")), 0, 0);
  gen_layout->addWidget(m_filename_ledit, 0, 1);
  gen_layout->addWidget(new QLabel(tr("Type:")), 1, 0);
  gen_layout->addWidget(m_type_cbox, 1, 1);
  gen_layout->addWidget(new QLabel(tr("Exposure (sec):")), 2, 0);
  gen_layout->addWidget(m_exposure_ledit, 2, 1);
  gen_layout->addWidget(new QLabel(tr("Num Images:")), 3, 0);
  gen_layout->addWidget(m_nimages_ledit, 3, 1);
  gen_gbox->setLayout(gen_layout);

  /* group buttons */
  QHBoxLayout *btn_hbox = new QHBoxLayout;
  btn_hbox->addWidget(m_start_button);
  btn_hbox->addWidget(m_cancel_button);

  QFrame *line = new QFrame();
  line->setFrameShape(QFrame::HLine);
  line->setFrameShadow(QFrame::Sunken);

  m_layout = new QVBoxLayout;
  m_layout->addWidget(gen_gbox);
  m_layout->addWidget(bin_gbox);
  m_layout->addWidget(pix_gbox);
  m_layout->addLayout(btn_hbox);
  // m_layout->setSizeConstraint(QLayout::SetFixedSize);

  // cancel signals, closes app
  /*connect(m_exit_button, SIGNAL(clicked()), QApplication::instance(),
          SLOT(quit()));*/
}
