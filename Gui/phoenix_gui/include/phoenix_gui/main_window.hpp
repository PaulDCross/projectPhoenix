/**
 * @file /include/phoenix_gui/main_window.hpp
 *
 * @brief Qt based gui for phoenix_gui.
 *
 * @date November 2010
 **/
#ifndef phoenix_gui_MAIN_WINDOW_H
#define phoenix_gui_MAIN_WINDOW_H

/*****************************************************************************
** Includes
*****************************************************************************/

#include <QtGui/QMainWindow>
#include "ui_main_window.h"
#include "qnode.hpp"

/*****************************************************************************
** Namespace
*****************************************************************************/

namespace phoenix_gui {

/*****************************************************************************
** Interface [MainWindow]
*****************************************************************************/
/**
 * @brief Qt central, all operations relating to the view part here.
 */
class MainWindow : public QMainWindow {
Q_OBJECT

public:
	MainWindow(int argc, char** argv, QWidget *parent = 0);
	~MainWindow();

	void ReadSettings(); // Load up qt program settings at startup
	void WriteSettings(); // Save qt program settings when closing

	void closeEvent(QCloseEvent *event); // Overloaded function
	void showNoMasterMessage();

public slots:
	/******************************************
	** Auto-connections (connectSlotsByName())
	*******************************************/
	
	void on_actionAbout_triggered();

	/* network setup slots */
	void on_button_connect_clicked(bool check );
	void on_checkbox_use_environment_stateChanged(int state);

	/* PID config slots */
	void on_pushButton_applyConfig_PID_clicked();
	void on_pushButton_saveConfig_PID_clicked();
	void on_pushButton_loadConfig_PID_clicked();

	/* sonar config slots */
	void on_pushButton_applyConfig_sonar_clicked();
	void on_pushButton_saveConfig_sonar_clicked();
	void on_pushButton_loadConfig_sonar_clicked();
	void on_spinBox_minDist_sonar_valueChanged(double arg1);
	void on_spinBox_maxDist_sonar_valueChanged(double arg1);
//	void on_spinBox_leftLimit_sonar_valueChanged(double arg1);
//	void on_spinBox_rightLimit_sonar_valueChanged(double arg1);
	void on_spinBox_threshold_sonar_valueChanged(double arg1);
	void on_spinBox_contrast_sonar_valueChanged(double arg1);

	
	/******************************************
	** Manual connections
	*******************************************/
	void noMaster();
private:
	Ui::MainWindowDesign ui;
	QNode qnode;
	QString m_sSettingsFile;
};

}  // namespace phoenix_gui

#endif // phoenix_gui_MAIN_WINDOW_H
