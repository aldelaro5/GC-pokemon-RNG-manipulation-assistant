#include "PokemonPropertiesFrame.h"

#include <QColor>
#include <QFont>
#include <QHBoxLayout>
#include <QPalette>
#include <QVBoxLayout>

#include "../GUICommon.h"

PokemonPropertiesFrame::PokemonPropertiesFrame(QString pokemonName, QPixmap pokemonIcon,
                                               QWidget* parent)
    : QFrame(parent)
{
  setFrameStyle(QFrame::Box | QFrame::Plain);
  // This is done because this frame is expected to be captured using a live streaming software with
  // cropping aligned with the frame; the size cannot change because of this
  setFixedWidth(360);
  setFixedHeight(100);

  // The default fonts were hard to read which would be a problem considering this is supposed to be
  // viewed on a live stream. Therefore, a bigger bold font is more appropriate.
  QFont theFont = font();
  theFont.setPointSize(12);
  theFont.setBold(true);
  setFont(theFont);

  m_lblHpIv = new QLabel("?");
  m_lblHpIv->setAlignment(Qt::AlignCenter);
  m_lblAtkIv = new QLabel("?");
  m_lblAtkIv->setAlignment(Qt::AlignCenter);
  m_lblDefIv = new QLabel("?");
  m_lblDefIv->setAlignment(Qt::AlignCenter);
  m_lblSpAtkIv = new QLabel("?");
  m_lblSpAtkIv->setAlignment(Qt::AlignCenter);
  m_lblSpDefIv = new QLabel("?");
  m_lblSpDefIv->setAlignment(Qt::AlignCenter);
  m_lblSpeedIv = new QLabel("?");
  m_lblSpeedIv->setAlignment(Qt::AlignCenter);

  m_lblGender = new QLabel("?");
  m_lblNature = new QLabel("???");
  m_lblHiddenPower = new QLabel("??? ?");

  m_lblName = new QLabel(pokemonName);
  m_lblIcon = new QLabel();
  m_lblIcon->setPixmap(pokemonIcon);

  QHBoxLayout* topLayout = new QHBoxLayout;
  topLayout->addWidget(m_lblIcon);
  topLayout->addWidget(m_lblName);
  topLayout->addWidget(m_lblGender);
  topLayout->addWidget(m_lblNature);
  topLayout->addWidget(m_lblHiddenPower);

  QLabel* lblHpIvHeader = new QLabel("HP");
  lblHpIvHeader->setAlignment(Qt::AlignCenter);
  QLabel* lblAtkIvHeader = new QLabel("Atk");
  lblAtkIvHeader->setAlignment(Qt::AlignCenter);
  QLabel* lblDefIvHeader = new QLabel("Def");
  lblDefIvHeader->setAlignment(Qt::AlignCenter);
  QLabel* lblSpAtkIvHeader = new QLabel("SpA");
  lblSpAtkIvHeader->setAlignment(Qt::AlignCenter);
  QLabel* lblSpDefIvHeader = new QLabel("SpD");
  lblSpDefIvHeader->setAlignment(Qt::AlignCenter);
  QLabel* lblSpeedIvHeader = new QLabel("Spe");
  lblSpeedIvHeader->setAlignment(Qt::AlignCenter);

  QHBoxLayout* ivsHeaderLayout = new QHBoxLayout;
  ivsHeaderLayout->addWidget(lblHpIvHeader);
  ivsHeaderLayout->addWidget(lblAtkIvHeader);
  ivsHeaderLayout->addWidget(lblDefIvHeader);
  ivsHeaderLayout->addWidget(lblSpAtkIvHeader);
  ivsHeaderLayout->addWidget(lblSpDefIvHeader);
  ivsHeaderLayout->addWidget(lblSpeedIvHeader);

  QHBoxLayout* ivsLayout = new QHBoxLayout;
  ivsLayout->addWidget(m_lblHpIv);
  ivsLayout->addWidget(m_lblAtkIv);
  ivsLayout->addWidget(m_lblDefIv);
  ivsLayout->addWidget(m_lblSpAtkIv);
  ivsLayout->addWidget(m_lblSpDefIv);
  ivsLayout->addWidget(m_lblSpeedIv);

  QVBoxLayout* mainLayout = new QVBoxLayout;
  mainLayout->addLayout(topLayout);
  mainLayout->addLayout(ivsHeaderLayout);
  mainLayout->addLayout(ivsLayout);
  // The 0s are for the icon to be flush with the corner which looks nicer with the border and the 5
  // is because on Windows, the text arrived too close to the border making an unpleasing effect.
  mainLayout->setContentsMargins(0, 0, 5, 3);

  setLayout(mainLayout);
}

void PokemonPropertiesFrame::setPokemonName(const QString name)
{
  m_lblName->setText(name);
}

void PokemonPropertiesFrame::setPokemonIcon(const QPixmap icon)
{
  m_lblIcon->setPixmap(icon);
}

void PokemonPropertiesFrame::setPokemonProperties(BaseRNGSystem::PokemonProperties properties)
{
  m_lblHpIv->setText(QString::number(properties.hpIV));
  m_lblAtkIv->setText(QString::number(properties.atkIV));
  m_lblDefIv->setText(QString::number(properties.defIV));
  m_lblSpAtkIv->setText(QString::number(properties.spAtkIV));
  m_lblSpDefIv->setText(QString::number(properties.spDefIV));
  m_lblSpeedIv->setText(QString::number(properties.speedIV));

  if (properties.genderIndex == 0)
    m_lblGender->setText("<font color='Blue'>♂</font>");
  else
    m_lblGender->setText("<font color='Red'>♀</font>");

  m_lblNature->setText(GUICommon::naturesStr[properties.natureIndex]);
  m_lblHiddenPower->setText(GUICommon::typesStr[properties.hiddenPowerTypeIndex] + QString(" ") +
                            QString::number(properties.hiddenPowerPower));

  if (properties.natureIndex % 6 != 0)
  {
    QPalette bluePalette = palette();
    bluePalette.setColor(QPalette::WindowText, Qt::blue);
    QPalette redPalette = palette();
    redPalette.setColor(QPalette::WindowText, Qt::red);

    int plusStat = (properties.natureIndex / 5) + 1;
    int minusStat = (properties.natureIndex % 5) + 1;
    switch (plusStat)
    {
    case 1:
      m_lblAtkIv->setPalette(redPalette);
      break;
    case 2:
      m_lblDefIv->setPalette(redPalette);
      break;
    case 3:
      m_lblSpeedIv->setPalette(redPalette);
      break;
    case 4:
      m_lblSpAtkIv->setPalette(redPalette);
      break;
    case 5:
      m_lblSpDefIv->setPalette(redPalette);
      break;
    }

    switch (minusStat)
    {
    case 1:
      m_lblAtkIv->setPalette(bluePalette);
      break;
    case 2:
      m_lblDefIv->setPalette(bluePalette);
      break;
    case 3:
      m_lblSpeedIv->setPalette(bluePalette);
      break;
    case 4:
      m_lblSpAtkIv->setPalette(bluePalette);
      break;
    case 5:
      m_lblSpDefIv->setPalette(bluePalette);
      break;
    }
  }
}

void PokemonPropertiesFrame::reset()
{
  m_lblHpIv->setText("?");
  m_lblHpIv->setPalette(palette());
  m_lblAtkIv->setText("?");
  m_lblAtkIv->setPalette(palette());
  m_lblDefIv->setText("?");
  m_lblDefIv->setPalette(palette());
  m_lblSpAtkIv->setText("?");
  m_lblSpAtkIv->setPalette(palette());
  m_lblSpDefIv->setText("?");
  m_lblSpDefIv->setPalette(palette());
  m_lblSpeedIv->setText("?");
  m_lblSpeedIv->setPalette(palette());

  m_lblGender->setText("?");
  m_lblGender->setPalette(palette());
  m_lblNature->setText("???");
  m_lblHiddenPower->setText("??? ?");
}
