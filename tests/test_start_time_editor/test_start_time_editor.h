/*
	SPDX-FileCopyrightText: 2026 Filipe Miranda

	SPDX-License-Identifier: GPL-3.0-or-later
*/

#ifndef KAPOW_TEST_START_TIME_EDITOR_H
#define KAPOW_TEST_START_TIME_EDITOR_H

#include <QObject>

class TestStartTimeEditor : public QObject
{
	Q_OBJECT

private Q_SLOTS:
	void enterConfirmsEditedStartTime();
	void escapeCancels();
	void focusLeavingEditorCancels();
	void focusMovingBetweenEditorsKeepsEditing();
	void focusMovingToAnotherWindowKeepsEditing();
	void clickingElsewhereCancels();
	void clickingEditorKeepsEditing();
	void switchingApplicationCancels();
	void hiddenEditorIgnoresSwitchingApplication();
};

#endif // KAPOW_TEST_START_TIME_EDITOR_H
