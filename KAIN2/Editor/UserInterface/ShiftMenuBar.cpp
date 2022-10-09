#include "ShiftMenuBar.h"

#include "UserInterface/Editor_UI.h"

Shift::MenuBar::MenuBar(QWidget* parent) : QMenuBar(parent)
{
    //Set the object name (used for custom styling).
    setObjectName("ShiftMenuBar");

    //Add menus.
	m_zoneMenu = addMenu(tr("Zone"));
    m_editMenu = addMenu(tr("Edit"));
    m_scriptsMenu = addMenu(tr("Scripts"));
    m_windowsMenu = addMenu(tr("Windows"));
    m_unsupportedMenu = addMenu(tr("Unsupported"));
    m_helpMenu = addMenu(tr("Help"));

    //Add actions.
    QAction* currentAction = m_zoneMenu->addAction(tr("Open"));
    connect(currentAction, SIGNAL(triggered()), this, SLOT(DoOpenZone()));
    currentAction = m_zoneMenu->addAction(tr("Close"));
    currentAction = m_zoneMenu->addAction(tr("Save"));

    m_labelVersion = new QLabel(this);
    char versionBuff[32];
    //sprintf(versionBuff, "V 1.0.%d (0x%x)", g_GIT_COMMIT_COUNT, g_GIT_COMMIT_COUNT);
    m_labelVersion->setText(versionBuff);
    m_labelVersion->move(340, 2);
    m_labelVersion->setStyleSheet("color:rgb(91, 91, 91);");

    m_labelWhatsNew = new QLabel(this);
    m_labelWhatsNew->setText("What's new");
    m_labelWhatsNew->move(460, 2);
    m_labelWhatsNew->setStyleSheet("color:rgb(91, 91, 91); text-decoration: underline;");
}

void Shift::MenuBar::DoOpenZone()
{
    m_zoneOpenDialog = new QDialog;
    m_zoneOpenDialog->setWindowTitle("Open");

    m_vbox = new QHBoxLayout;
    m_groupBox = new QGroupBox;
    m_groupBox->setTitle("Open Zone");
    m_zoneOpenDialog->show();

    m_labelZone = new QLabel(m_groupBox);
    m_labelZone->setText("Zone:");
    m_zoneComboBox = new QComboBox(m_groupBox);

    const char* zoneNames[] = {
                "ac_bunker",
                "ac_challenge_tomb",
                "ac_dummy_tomb",
                "ac_forest",
                "ac_main",
                "ac_main_to_swamp_connector",
                "bh_beach_hub",
                "bh_sidetomb_connector_01",
                "bh_sidetomb_connector_02",
                "bi_altar_room",
                "bi_catacombs",
                "bi_ceremony",
                "bi_entrance",
                "bi_exit",
                "bi_pit",
                "bi_puzzle",
                "ch_cine_transformation",
                "ch_hubtochasm",
                "chasm_bridge",
                "chasm_entrance",
                "chasm_streamhall_01",
                "chasm_streamhall_02",
                "cine_chaos_beach",
                "co_op_ww2_sos_01",
                "co_op_ww2_sos_02",
                "co_op_ww2_sos_03",
                "co_op_ww2_sos_04",
                "co_op_ww2_sos_gas_puzzle",
                "co_op_ww2_sos_map_room",
                "co_op_ww2_sos_pipes",
                "connector_acmain_to_mountainclimb_a",
                "coop_01_catacombs_pit",
                "coop_01_village",
                "coop_02_catacombs",
                "coop_03_catacombs_altar",
                "coop_04_catacombs_puzzle",
                "coop_04_temple",
                "coop_05_marsh",
                "ct_batcave",
                "ct_fortress_of_solitude",
                "ct_windchasm",
                "ct_windchasm_connector",
                "de_descent",
                "de_descent_to_scav_hub_connector",
                "ge_01",
                "ge_02",
                "ge_02_a",
                "ge_03",
                "ge_04",
                "ge_05",
                "ge_06",
                "ge_07",
                "ge_08",
                "ge_incense_burner_destructable_collection",
                "ma_chasm_to_hub_connector",
                "ma_chasm_vista",
                "ma_monastery_interior",
                "ma_puzzle",
                "ma_run_out",
                "main_menu_1",
                "mb_candlehall_combat",
                "mb_eatery",
                "mb_readyroom",
                "mountain_climb",
                "mountain_climb_to_village_hub_connector",
                "net_test_collectible_ahan",
                "pdlc_30d_bunker",
                "pdlc_30d_forest",
                "pdlc_60d_lost_fleet",
                "pdlc_60d_ritual",
                "pedestal_bat",
                "pedestal_chicken",
                "pedestal_crab",
                "pedestal_crow",
                "pedestal_fish",
                "pedestal_frog",
                "pedestal_grim",
                "pedestal_jonah",
                "pedestal_lara",
                "pedestal_oni_soldier",
                "pedestal_oni_stalker",
                "pedestal_rabbit",
                "pedestal_rat",
                "pedestal_roth",
                "pedestal_sam",
                "pedestal_scavenger_archer",
                "pedestal_scavenger_melee",
                "pedestal_scavenger_priest",
                "pedestal_scavenger_shield",
                "pedestal_scavenger_tank",
                "ptboat_cine",
                "qt_hall_of_queens",
                "qt_pre_stalker_arena",
                "qt_scale_the_ziggurat",
                "qt_stalkerfight",
                "qt_the_ritual",
                "qt_trial_by_fire",
                "qt_zig_to_ritual_connector",
                "rc_01_marsh",
                "rc_15_camp",
                "rc_20_wolfden",
                "rc_95_sound",
                "rc_sidetomb_connector",
                "sb_01",
                "sb_05",
                "sb_15",
                "sb_16",
                "sb_17",
                "sb_20",
                "sb_21",
                "sb_water_death",
                "sh_scavenger_hub",
                "sh_scavenger_hub_2",
                "sh_scavenger_hub_to_chopper_connector",
                "sh_scavenger_hub_to_geothermal_connector",
                "sh_scavenger_hub_to_well_connector",
                "shipendcinematic",
                "si_05_bunker_to_research_connector",
                "si_10_research",
                "si_15_elevator",
                "si_20_machinery",
                "si_25_tomb",
                "si_30_tomb_to_bh_connector",
                "si_35_bh_to_restroom_connector",
                "si_55_submarine_to_bh_connector",
                "si_95_sound",
                "si_elevator",
                "si_onigeneral_tomb",
                "si_research",
                "so_vistaview_global",
                "survival_den_puzzleroom",
                "survival_den_rambotunnel",
                "tb_skub_to_kick_the_bucket",
                "tb_to_beach",
                "tb_to_beach_to_beach_hub_connector",
                "tedcampsite",
                "tedpersistentdata",
                "tedscratchpad",
                "testlevel1",
                "testmap",
                "tr_01_scavhub",
                "tr_02_ww2_beach",
                "tr_03_japanese_shrine",
                "tr_04_chasm_bridge",
                "tr_05_submarine",
                "tr_06_monastery",
                "tr_07_great_escape",
                "tr_08_mountain_village",
                "tr_09_marsh",
                "tr_11_lost_fleet",
                "tr_12_forest",
                "tr_13_caves",
                "tr_14_yousai",
                "tr_16_the_cove",
                "tr_17_scavhub_c",
                "tt_connector_to_rc_01_marsh",
                "tt_two_towers",
                "unknown",
                "vc_fishery",
                "vc_plane_chopshop",
                "vc_shockpond",
                "vc_well_outsource",
                "vc_wolf_paddock",
                "vh_chasm_to_hub_connector",
                "vh_chopshop_connector",
                "vh_cliffs_to_hub_connector_a",
                "vh_cliffs_to_hub_connector_b",
                "vh_fisheries_connector",
                "vh_hub_to_chasm_connector",
                "vh_main",
                "vh_vhmain_to_descent_connector",
                "vh_vhmain_to_ww2_sos_01_connector",
                "vh_wolf_paddock_connector",
                "ww2_sos_01",
                "ww2_sos_02",
                "ww2sos_03",
                "ww2sos_03_to_04_connector",
                "ww2sos_04",
                "ww2sos_05",
                "ww2sos_gas_puzzle",
                "ww2sos_map_room",
                "cliffs_of_insanity",
                "oceanvista",
                "slide_of_insanity",
                "cinefx",
                "container1",
                "test_leveleditor1",
                "bridge_onslaught_start",
                "survival_den03",
                "survival_den04",
                "survival_den97",
                "test"
    };

    for (int i = 0; i < sizeof(zoneNames) / sizeof(uintptr_t); i++)
    {
        m_zoneComboBox->addItem(tr(zoneNames[i]));
    }
    QPushButton* openButton = new QPushButton(tr("Open"), m_groupBox);
    m_labelZone->setGeometry(30, 30, 200, 26);
    m_zoneComboBox->setGeometry(90, 30, 200, 26);
    openButton->setGeometry(320, 30, 32, 32);
    m_vbox->addWidget(m_groupBox);
    m_zoneOpenDialog->setLayout(m_vbox);
    m_zoneComboBox->show();
    connect(openButton, SIGNAL(clicked()), this, SLOT(OpenZone()));
}

void Shift::MenuBar::OpenZone()
{
    //g_engine.loadZone(m_zoneComboBox->currentText().toLocal8Bit().data());
    Editor::UI::InitialiseCamera();
}
