#include "ShiftToolBar.h"

#include <QComboBox>

Shift::ToolBar::ToolBar(QWidget* parent) : QToolBar(parent)
{
	//Main toolbar properties and setup.
	setMovable(false);
	setObjectName(tr("ShiftToolBar"));
	setIconSize(QSize(42, 42));
	setMinimumHeight(38);
	setSizePolicy(QSizePolicy::Fixed, QSizePolicy::MinimumExpanding);
	
	m_mouseButton = new Shift::ToolButton;
	m_mouseButton->setObjectName(tr("mouse"));
	m_mouseButton->setIcon(QIcon(":/Dark/Toolbar1.png"));
	m_mouseButton->setChecked(true);
	m_mouseButton->connect(m_mouseButton, SIGNAL(clicked()), SLOT(OnClick()));
	m_mouseButton->move(32, m_mouseButton->y());
	m_mouseButton->setMinimumSize(26, 26);
	m_mouseButton->setMaximumSize(26, 26);
	addWidget(m_mouseButton);

	addSeparator();

	m_translateButton = new Shift::ToolButton;
	m_translateButton->setObjectName(tr("translate"));
	m_translateButton->setIcon(QIcon(":/Dark/Toolbar2.png"));
	m_translateButton->setChecked(true);
	m_translateButton->connect(m_translateButton, SIGNAL(clicked()), SLOT(OnClick()));
	m_translateButton->setMinimumSize(26, 26);
	m_translateButton->setMaximumSize(26, 26);
	addWidget(m_translateButton);

	m_rotateButton = new Shift::ToolButton;
	m_rotateButton->setObjectName(tr("rotate"));
	m_rotateButton->setIcon(QIcon(":/Dark/Toolbar3.png"));
	m_rotateButton->setChecked(true);
	m_rotateButton->connect(m_rotateButton, SIGNAL(clicked()), SLOT(OnClick()));
	m_rotateButton->setMinimumSize(26, 26);
	m_rotateButton->setMaximumSize(26, 26);
	addWidget(m_rotateButton);

	m_scaleButton = new Shift::ToolButton;
	m_scaleButton->setObjectName(tr("scale"));
	m_scaleButton->setIcon(QIcon(":/Dark/Toolbar4.png"));
	m_scaleButton->setChecked(true);
	m_scaleButton->connect(m_scaleButton, SIGNAL(clicked()), SLOT(OnClick()));
	m_scaleButton->setMinimumSize(26, 26);
	m_scaleButton->setMaximumSize(26, 26);
	addWidget(m_scaleButton);

	addSeparator();

	m_toolButton5 = new Shift::ToolButton;
	m_toolButton5->setObjectName(tr("toolbar5"));
	m_toolButton5->setIcon(QIcon(":/Dark/Toolbar5.png"));
	m_toolButton5->setChecked(true);
	m_toolButton5->connect(m_toolButton5, SIGNAL(clicked()), SLOT(OnClick()));
	m_toolButton5->setMinimumSize(26, 26);
	m_toolButton5->setMaximumSize(26, 26);
	addWidget(m_toolButton5);

	addSeparator();

	QComboBox* comboBox1 = new QComboBox();
	comboBox1->addItem(tr("World"));
	comboBox1->addItem(tr("Local"));
	comboBox1->setMinimumSize(68, 24);
	comboBox1->setMaximumSize(68, 24);
	addWidget(comboBox1);

	addSeparator();

	m_toolButton6 = new Shift::ToolButton;
	m_toolButton6->setObjectName(tr("toolbar6"));
	m_toolButton6->setIcon(QIcon(":/Dark/Toolbar6.png"));
	m_toolButton6->setChecked(true);
	m_toolButton6->connect(m_toolButton6, SIGNAL(clicked()), SLOT(OnClick()));
	m_toolButton6->setMinimumSize(26, 26);
	m_toolButton6->setMaximumSize(26, 26);
	addWidget(m_toolButton6);

	m_toolButton7 = new Shift::ToolButton;
	m_toolButton7->setObjectName(tr("toolbar7"));
	m_toolButton7->setIcon(QIcon(":/Dark/Toolbar7.png"));
	m_toolButton7->setChecked(true);
	m_toolButton7->connect(m_toolButton7, SIGNAL(clicked()), SLOT(OnClick()));
	m_toolButton7->setMinimumSize(26, 26);
	m_toolButton7->setMaximumSize(26, 26);
	addWidget(m_toolButton7);

	addSeparator();
	addSeparator();

	m_toolButton8 = new Shift::ToolButton;
	m_toolButton8->setObjectName(tr("toolbar8"));
	m_toolButton8->setIcon(QIcon(":/Dark/Toolbar8.png"));
	m_toolButton8->setChecked(true);
	m_toolButton8->connect(m_toolButton8, SIGNAL(clicked()), SLOT(OnClick()));
	m_toolButton8->connect(m_toolButton8, SIGNAL(pressed()), this, SLOT(ToggleGrid()));
	m_toolButton8->setMinimumSize(26, 26);
	m_toolButton8->setMaximumSize(26, 26);
	addWidget(m_toolButton8);

	m_toolButton9 = new Shift::ToolButton;
	m_toolButton9->setObjectName(tr("toolbar9"));
	m_toolButton9->setIcon(QIcon(":/Dark/Toolbar9.png"));
	m_toolButton9->setChecked(true);
	m_toolButton9->connect(m_toolButton9, SIGNAL(clicked()), SLOT(OnClick()));
	m_toolButton9->setMinimumSize(26, 26);
	m_toolButton9->setMaximumSize(26, 26);
	addWidget(m_toolButton9);

	m_toolButton10 = new Shift::ToolButton;
	m_toolButton10->setObjectName(tr("toolbar10"));
	m_toolButton10->setIcon(QIcon(":/Dark/Toolbar10.png"));
	m_toolButton10->setChecked(true);
	m_toolButton10->connect(m_toolButton10, SIGNAL(clicked()), SLOT(OnClick()));
	m_toolButton10->setMinimumSize(26, 26);
	m_toolButton10->setMaximumSize(26, 26);
	addWidget(m_toolButton10);

	addSeparator();
	addSeparator();

	m_toolButton11 = new Shift::ToolMenuButton;
	m_toolButton11->setObjectName(tr("toolbar11"));
	m_toolButton11->setIcon(QIcon(":/Dark/Toolbar11.png"));
	m_toolButton11->setChecked(true);
	m_toolButton11->connect(m_toolButton11, SIGNAL(clicked()), SLOT(OnClick()));
	m_toolButton11->setMinimumSize(26, 26);
	m_toolButton11->setMaximumSize(26, 26);

	m_menu1 = new QMenu;
	m_menu1->addAction(tr("Test"));
	m_menu1->setAttribute(Qt::WA_TranslucentBackground);
	m_menu1->setWindowFlags(m_menu1->windowFlags() | Qt::FramelessWindowHint);
	m_toolButton11->setMenu(m_menu1);
	m_toolButton11->setPopupMode(QToolButton::InstantPopup);

	addWidget(m_toolButton11);

	m_toolButton12 = new Shift::ToolMenuButton;
	m_toolButton12->setObjectName(tr("toolbar12"));
	m_toolButton12->setIcon(QIcon(":/Dark/Toolbar12.png"));
	m_toolButton12->setChecked(true);
	m_toolButton12->connect(m_toolButton12, SIGNAL(clicked()), SLOT(OnClick()));
	m_toolButton12->setMinimumSize(26, 26);
	m_toolButton12->setMaximumSize(26, 26);
	m_menu2 = new QMenu;
	m_menu2->addAction(tr("Test"));
	m_menu2->setAttribute(Qt::WA_TranslucentBackground);
	m_menu2->setWindowFlags(m_menu2->windowFlags() | Qt::FramelessWindowHint);
	m_toolButton12->setMenu(m_menu2);
	m_toolButton12->setPopupMode(QToolButton::InstantPopup);
	addWidget(m_toolButton12);

	m_toolButton13 = new Shift::ToolMenuButton;
	m_toolButton13->setObjectName(tr("toolbar13"));
	m_toolButton13->setIcon(QIcon(":/Dark/Toolbar13.png"));
	m_toolButton13->setChecked(true);
	m_toolButton13->connect(m_toolButton13, SIGNAL(clicked()), SLOT(OnClick()));
	m_toolButton13->setMinimumSize(26, 26);
	m_toolButton13->setMaximumSize(26, 26);
	m_menu3 = new QMenu;
	m_menu3->addAction(tr("Test"));
	m_menu3->setAttribute(Qt::WA_TranslucentBackground);
	m_menu3->setWindowFlags(m_menu3->windowFlags() | Qt::FramelessWindowHint);
	m_toolButton13->setMenu(m_menu3);
	m_toolButton13->setPopupMode(QToolButton::InstantPopup);
	addWidget(m_toolButton13);

	m_toolButton14 = new Shift::ToolMenuButton;
	m_toolButton14->setObjectName(tr("toolbar14"));
	m_toolButton14->setIcon(QIcon(":/Dark/Toolbar14.png"));
	m_toolButton14->setChecked(true);
	m_toolButton14->connect(m_toolButton14, SIGNAL(clicked()), SLOT(OnClick()));
	m_toolButton14->setMinimumSize(26, 26);
	m_toolButton14->setMaximumSize(26, 26);
	m_menu4 = new QMenu;
	m_menu4->addAction(tr("Test"));
	m_menu4->setAttribute(Qt::WA_TranslucentBackground);
	m_menu4->setWindowFlags(m_menu4->windowFlags() | Qt::FramelessWindowHint);
	m_toolButton14->setMenu(m_menu4);
	m_toolButton14->setPopupMode(QToolButton::InstantPopup);
	addWidget(m_toolButton14);

	m_toolButton15 = new Shift::ToolMenuButton;
	m_toolButton15->setObjectName(tr("toolbar15"));
	m_toolButton15->setIcon(QIcon(":/Dark/Toolbar15.png"));
	m_toolButton15->setChecked(true);
	m_toolButton15->connect(m_toolButton15, SIGNAL(clicked()), SLOT(OnClick()));
	m_toolButton15->setMinimumSize(26, 26);
	m_toolButton15->setMaximumSize(26, 26);
	m_menu5 = new QMenu;
	m_menu5->addAction(tr("Test"));
	m_menu5->setAttribute(Qt::WA_TranslucentBackground);
	m_menu5->setWindowFlags(m_menu5->windowFlags() | Qt::FramelessWindowHint);
	m_toolButton15->setMenu(m_menu5);
	m_toolButton15->setPopupMode(QToolButton::InstantPopup);
	addWidget(m_toolButton15);

	m_toolButton16 = new Shift::ToolMenuButton;
	m_toolButton16->setObjectName(tr("toolbar16"));
	m_toolButton16->setIcon(QIcon(":/Dark/Toolbar16.png"));
	m_toolButton16->setChecked(true);
	m_toolButton16->connect(m_toolButton16, SIGNAL(clicked()), SLOT(OnClick()));
	m_toolButton16->setMinimumSize(26, 26);
	m_toolButton16->setMaximumSize(26, 26);
	m_menu6 = new QMenu;
	m_menu6->addAction(tr("Test"));
	m_menu6->setAttribute(Qt::WA_TranslucentBackground);
	m_menu6->setWindowFlags(m_menu6->windowFlags() | Qt::FramelessWindowHint);
	m_toolButton16->setMenu(m_menu6);
	m_toolButton16->setPopupMode(QToolButton::InstantPopup);
	addWidget(m_toolButton16);

	m_toolButton17 = new Shift::ToolMenuButton;
	m_toolButton17->setObjectName(tr("toolbar17"));
	m_toolButton17->setIcon(QIcon(":/Dark/Toolbar17.png"));
	m_toolButton17->setChecked(true);
	m_toolButton17->connect(m_toolButton17, SIGNAL(clicked()), SLOT(OnClick()));
	m_toolButton17->setMinimumSize(26, 26);
	m_toolButton17->setMaximumSize(26, 26);
	m_menu7 = new QMenu;
	m_menu7->addAction(tr("Test"));
	m_menu7->setAttribute(Qt::WA_TranslucentBackground);
	m_menu7->setWindowFlags(m_menu7->windowFlags() | Qt::FramelessWindowHint);
	m_toolButton17->setMenu(m_menu7);
	m_toolButton17->setPopupMode(QToolButton::InstantPopup);
	addWidget(m_toolButton17);

	m_toolButton18 = new Shift::ToolMenuButton;
	m_toolButton18->setObjectName(tr("toolbar18"));
	m_toolButton18->setIcon(QIcon(":/Dark/Toolbar18.png"));
	m_toolButton18->setChecked(true);
	m_toolButton18->connect(m_toolButton18, SIGNAL(clicked()), SLOT(OnClick()));
	m_toolButton18->setMinimumSize(26, 26);
	m_toolButton18->setMaximumSize(26, 26);
	m_menu8 = new QMenu;
	m_menu8->addAction(tr("Test"));
	m_menu8->setAttribute(Qt::WA_TranslucentBackground);
	m_menu8->setWindowFlags(m_menu8->windowFlags() | Qt::FramelessWindowHint);
	m_toolButton18->setMenu(m_menu8);
	m_toolButton18->setPopupMode(QToolButton::InstantPopup);
	addWidget(m_toolButton18);

	m_toolButton19 = new Shift::ToolMenuButton;
	m_toolButton19->setObjectName(tr("toolbar19"));
	m_toolButton19->setIcon(QIcon(":/Dark/Toolbar19.png"));
	m_toolButton19->setChecked(true);
	m_toolButton19->connect(m_toolButton19, SIGNAL(clicked()), SLOT(OnClick()));
	m_toolButton19->setMinimumSize(26, 26);
	m_toolButton19->setMaximumSize(26, 26);
	m_menu9 = new QMenu;
	m_menu9->addAction(tr("Test"));
	m_menu9->setAttribute(Qt::WA_TranslucentBackground);
	m_menu9->setWindowFlags(m_menu9->windowFlags() | Qt::FramelessWindowHint);
	m_toolButton19->setMenu(m_menu9);
	m_toolButton19->setPopupMode(QToolButton::InstantPopup);
	addWidget(m_toolButton19);

	m_toolButton20 = new Shift::ToolMenuButton;
	m_toolButton20->setObjectName(tr("toolbar20"));
	m_toolButton20->setIcon(QIcon(":/Dark/Toolbar20.png"));
	m_toolButton20->setChecked(true);
	m_toolButton20->connect(m_toolButton20, SIGNAL(clicked()), SLOT(OnClick()));
	m_toolButton20->setMinimumSize(26, 26);
	m_toolButton20->setMaximumSize(26, 26);
	m_menu10 = new QMenu;
	m_menu10->addAction(tr("Test"));
	m_menu10->setAttribute(Qt::WA_TranslucentBackground);
	m_menu10->setWindowFlags(m_menu10->windowFlags() | Qt::FramelessWindowHint);
	m_toolButton20->setMenu(m_menu10);
	m_toolButton20->setPopupMode(QToolButton::InstantPopup);
	addWidget(m_toolButton20);

	m_toolButton21 = new Shift::ToolMenuButton;
	m_toolButton21->setObjectName(tr("toolbar21"));
	m_toolButton21->setIcon(QIcon(":/Dark/Toolbar21.png"));
	m_toolButton21->setChecked(true);
	m_toolButton21->connect(m_toolButton21, SIGNAL(clicked()), SLOT(OnClick()));
	m_toolButton21->setMinimumSize(26, 26);
	m_toolButton21->setMaximumSize(26, 26);
	m_menu11 = new QMenu;
	m_menu11->addAction(tr("Test"));
	m_menu11->setAttribute(Qt::WA_TranslucentBackground);
	m_menu11->setWindowFlags(m_menu11->windowFlags() | Qt::FramelessWindowHint);
	m_toolButton21->setMenu(m_menu11);
	m_toolButton21->setPopupMode(QToolButton::InstantPopup);
	addWidget(m_toolButton21);

	m_toolButton22 = new Shift::ToolMenuButton;
	m_toolButton22->setObjectName(tr("toolbar22"));
	m_toolButton22->setIcon(QIcon(":/Dark/Toolbar22.png"));
	m_toolButton22->setChecked(true);
	m_toolButton22->connect(m_toolButton22, SIGNAL(clicked()), SLOT(OnClick()));
	m_toolButton22->setMinimumSize(26, 26);
	m_toolButton22->setMaximumSize(26, 26);
	m_menu12 = new QMenu;
	m_menu12->addAction(tr("Test"));
	m_menu12->setAttribute(Qt::WA_TranslucentBackground);
	m_menu12->setWindowFlags(m_menu12->windowFlags() | Qt::FramelessWindowHint);
	m_toolButton22->setMenu(m_menu12);
	m_toolButton22->setPopupMode(QToolButton::InstantPopup);
	addWidget(m_toolButton22);

	m_toolButton23 = new Shift::ToolMenuButton;
	m_toolButton23->setObjectName(tr("toolbar23"));
	m_toolButton23->setIcon(QIcon(":/Dark/Toolbar23.png"));
	m_toolButton23->setChecked(true);
	m_toolButton23->connect(m_toolButton23, SIGNAL(clicked()), SLOT(OnClick()));
	m_toolButton23->setMinimumSize(26, 26);
	m_toolButton23->setMaximumSize(26, 26);
	m_menu13 = new QMenu;
	m_menu13->addAction(tr("Test"));
	m_menu13->setAttribute(Qt::WA_TranslucentBackground);
	m_menu13->setWindowFlags(m_menu13->windowFlags() | Qt::FramelessWindowHint);
	m_toolButton23->setMenu(m_menu13);
	m_toolButton23->setPopupMode(QToolButton::InstantPopup);
	addWidget(m_toolButton23);

	m_toolButton24 = new Shift::ToolMenuButton;
	m_toolButton24->setObjectName(tr("toolbar24"));
	m_toolButton24->setIcon(QIcon(":/Dark/Toolbar24.png"));
	m_toolButton24->setChecked(true);
	m_toolButton24->connect(m_toolButton24, SIGNAL(clicked()), SLOT(OnClick()));
	m_toolButton24->setMinimumSize(26, 26);
	m_toolButton24->setMaximumSize(26, 26);
	m_menu14 = new QMenu;
	m_menu14->addAction(tr("Test"));
	m_menu14->setAttribute(Qt::WA_TranslucentBackground);
	m_menu14->setWindowFlags(m_menu14->windowFlags() | Qt::FramelessWindowHint);
	m_toolButton24->setMenu(m_menu14);
	m_toolButton24->setPopupMode(QToolButton::InstantPopup);
	addWidget(m_toolButton24);

	m_toolButton25 = new Shift::ToolMenuButton;
	m_toolButton25->setObjectName(tr("toolbar25"));
	m_toolButton25->setIcon(QIcon(":/Dark/Toolbar25.png"));
	m_toolButton25->setChecked(true);
	m_toolButton25->connect(m_toolButton25, SIGNAL(clicked()), SLOT(OnClick()));
	m_toolButton25->setMinimumSize(26, 26);
	m_toolButton25->setMaximumSize(26, 26);
	m_menu15 = new QMenu;
	m_menu15->addAction(tr("All"));
	m_menu15->addAction(tr("animspline"));
	m_menu15->addAction(tr("fxpointset"));
	m_menu15->addAction(tr("genericspline"));
	m_menu15->addAction(tr("jumphelper"));
	m_menu15->addAction(tr("jumphelper_jumptoledge"));
	m_menu15->addAction(tr("jumphelper_zipline"));
	m_menu15->addAction(tr("markup"));
	m_menu15->addAction(tr("markup_beam"));
	m_menu15->addAction(tr("markup_constrainedground"));
	m_menu15->addAction(tr("markup_cover"));
	m_menu15->addAction(tr("markup_edgestop"));
	m_menu15->addAction(tr("markup_ledge"));
	m_menu15->addAction(tr("markup_zipline"));
	m_menu15->addAction(tr("watercurrentspline"));
	m_menu15->addAction(tr("waypointset"));
	m_menu15->setAttribute(Qt::WA_TranslucentBackground);
	m_menu15->setWindowFlags(m_menu15->windowFlags() | Qt::FramelessWindowHint);
	m_toolButton25->setMenu(m_menu15);
	m_toolButton25->setPopupMode(QToolButton::InstantPopup);
	addWidget(m_toolButton25);

	m_toolButton26 = new Shift::ToolMenuButton;
	m_toolButton26->setObjectName(tr("toolbar26"));
	m_toolButton26->setIcon(QIcon(":/Dark/Toolbar26.png"));
	m_toolButton26->setChecked(true);
	m_toolButton26->connect(m_toolButton26, SIGNAL(clicked()), SLOT(OnClick()));
	m_toolButton26->setMinimumSize(26, 26);
	m_toolButton26->setMaximumSize(26, 26);
	m_menu16 = new QMenu;
	m_menu16->addAction(tr("plannerexclusionvolume"));
	m_menu16->addAction(tr("plannerobstacle"));
	m_menu16->addAction(tr("plannervolume"));
	m_menu16->addAction(tr("playersensor"));
	m_menu16->addAction(tr("soundemittervolume"));
	m_menu16->addAction(tr("soundtriggervolume"));
	m_menu16->addAction(tr("soundvolume"));
	m_menu16->addAction(tr("spatialvolume"));
	m_menu16->addAction(tr("triggerplane"));
	m_menu16->addAction(tr("triggerplane_fire_wall"));
	m_menu16->addAction(tr("triggerplane_mp"));
	m_menu16->addAction(tr("triggerplane_sound"));
	m_menu16->addAction(tr("triggersphere"));
	m_menu16->addAction(tr("triggersphere_fire"));
	m_menu16->addAction(tr("triggersphere_fire_snuff"));
	m_menu16->addAction(tr("triggersphere_fire_mp"));
	m_menu16->addAction(tr("triggervolume"));
	m_menu16->addAction(tr("triggervolume_camera"));
	m_menu16->addAction(tr("triggervolume_combat"));
	m_menu16->addAction(tr("triggervolume_fire_snuff"));
	m_menu16->addAction(tr("triggervolume_fire_wall"));
	m_menu16->addAction(tr("triggervolume_fire_wall_knockback"));
	m_menu16->addAction(tr("triggervolume_mp"));
	m_menu16->addAction(tr("triggervolume_sound"));
	m_menu16->addAction(tr("watervolume"));
	m_menu16->setAttribute(Qt::WA_TranslucentBackground);
	m_menu16->setWindowFlags(m_menu16->windowFlags() | Qt::FramelessWindowHint);
	m_toolButton26->setMenu(m_menu16);
	m_toolButton26->setPopupMode(QToolButton::InstantPopup);
	addWidget(m_toolButton26);

	m_toolButton27 = new Shift::ToolMenuButton;
	m_toolButton27->setObjectName(tr("toolbar27"));
	m_toolButton27->setIcon(QIcon(":/Dark/Toolbar27.png"));
	m_toolButton27->setChecked(true);
	m_toolButton27->connect(m_toolButton27, SIGNAL(clicked()), SLOT(OnClick()));
	m_toolButton27->setMinimumSize(26, 26);
	m_toolButton27->setMaximumSize(26, 26);
	m_menu17 = new QMenu;
	m_menu17->addAction(tr("Test"));
	m_menu17->setAttribute(Qt::WA_TranslucentBackground);
	m_menu17->setWindowFlags(m_menu17->windowFlags() | Qt::FramelessWindowHint);
	m_toolButton27->setMenu(m_menu17);
	m_toolButton27->setPopupMode(QToolButton::InstantPopup);
	addWidget(m_toolButton27);
}

void Shift::ToolBar::ToggleGrid()
{
	//g_engine.getRenderer()->m_drawGrid ^= true;
}

Shift::ToolBar::~ToolBar()
{
	if (m_mouseButton != nullptr)
	{
		delete m_mouseButton;
	}

	if (m_translateButton != nullptr)
	{
		delete m_translateButton;
	}
	
	if (m_rotateButton != nullptr)
	{
		delete m_rotateButton;
	}
	
	if (m_scaleButton != nullptr)
	{
		delete m_scaleButton;
	}
	
	if (m_toolButton5 != nullptr)
	{
		delete m_toolButton5;
	}

	if (m_toolButton6 != nullptr)
	{
		delete m_toolButton6;
	}

	if (m_toolButton7 != nullptr)
	{
		delete m_toolButton7;
	}

	if (m_toolButton8 != nullptr)
	{
		delete m_toolButton8;
	}

	if (m_toolButton9 != nullptr)
	{
		delete m_toolButton9;
	}

	if (m_toolButton10 != nullptr)
	{
		delete m_toolButton10;
	}

	if (m_toolButton11 != nullptr)
	{
		delete m_toolButton11;
	}

	if (m_toolButton12 != nullptr)
	{
		delete m_toolButton12;
	}

	if (m_toolButton13 != nullptr)
	{
		delete m_toolButton13;
	}

	if (m_toolButton14 != nullptr)
	{
		delete m_toolButton14;
	}

	if (m_toolButton15 != nullptr)
	{
		delete m_toolButton15;
	}

	if (m_toolButton16 != nullptr)
	{
		delete m_toolButton16;
	}

	if (m_toolButton17 != nullptr)
	{
		delete m_toolButton17;
	}

	if (m_toolButton18 != nullptr)
	{
		delete m_toolButton18;
	}

	if (m_toolButton19 != nullptr)
	{
		delete m_toolButton19;
	}

	if (m_toolButton20 != nullptr)
	{
		delete m_toolButton20;
	}

	if (m_toolButton21 != nullptr)
	{
		delete m_toolButton21;
	}

	if (m_toolButton22 != nullptr)
	{
		delete m_toolButton22;
	}

	if (m_toolButton23 != nullptr)
	{
		delete m_toolButton23;
	}

	if (m_toolButton24 != nullptr)
	{
		delete m_toolButton24;
	}

	if (m_toolButton25 != nullptr)
	{
		delete m_toolButton25;
	}

	if (m_toolButton26 != nullptr)
	{
		delete m_toolButton26;
	}

	if (m_toolButton27 != nullptr)
	{
		delete m_toolButton27;
	}

	if (m_menu1 != nullptr)
	{
		delete m_menu1;
	}

	if (m_menu2 != nullptr)
	{
		delete m_menu2;
	}

	if (m_menu3 != nullptr)
	{
		delete m_menu3;
	}

	if (m_menu4 != nullptr)
	{
		delete m_menu4;
	}

	if (m_menu5 != nullptr)
	{
		delete m_menu5;
	}

	if (m_menu6 != nullptr)
	{
		delete m_menu6;
	}

	if (m_menu7 != nullptr)
	{
		delete m_menu7;
	}

	if (m_menu8 != nullptr)
	{
		delete m_menu8;
	}

	if (m_menu9 != nullptr)
	{
		delete m_menu9;
	}

	if (m_menu10 != nullptr)
	{
		delete m_menu10;
	}

	if (m_menu11 != nullptr)
	{
		delete m_menu11;
	}

	if (m_menu12 != nullptr)
	{
		delete m_menu12;
	}

	if (m_menu13 != nullptr)
	{
		delete m_menu13;
	}

	if (m_menu14 != nullptr)
	{
		delete m_menu14;
	}

	if (m_menu15 != nullptr)
	{
		delete m_menu15;
	}

	if (m_menu16 != nullptr)
	{
		delete m_menu16;
	}

	if (m_menu17 != nullptr)
	{
		delete m_menu17;
	}
}
