/*
	SPDX-FileCopyrightText: 2026 Filipe Miranda

	SPDX-License-Identifier: GPL-3.0-or-later
*/

#ifndef KAPOW_START_TIME_DIALOG_H
#define KAPOW_START_TIME_DIALOG_H

class DateEditor;
class TimeEditor;

#include <QDateTime>
#include <QDialog>

class StartTimeDialog : public QDialog
{
	Q_OBJECT

public:
	explicit StartTimeDialog(QWidget* parent = nullptr);

	QDateTime startTime() const;
	void setStartTime(const QDateTime& start);

private:
	DateEditor* m_date;
	TimeEditor* m_time;
};

#endif // KAPOW_START_TIME_DIALOG_H
