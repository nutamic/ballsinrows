#include <QFile>
#include <QSaveFile>
#include <QTableWidget>
#include <QXmlStreamReader>
#include "records.h"
#include "appconfig.h"
#include "appdata.h"
constexpr char tagHighscores[] = "highscores";
constexpr char tagHighscore[] = "highscore";
constexpr char propertyValue[] = "value";
constexpr char tagGamer[] = "gamer";
constexpr char tagTime[] = "time";
constexpr char timeFormat[] = "dd.MM.yyyy HH:mm:ss";
bool verifyFile(QXmlStreamReader *reader);
Records::Records(short load){
	QFile file(AppData::records());
	if(!file.open(QIODevice::ReadOnly)) return;
	QXmlStreamReader reader(&file);
	if(!verifyFile(&reader)){
		file.close();
		return;
	}
	while(load-- ? reader.readNextStartElement() : false){
		Record record;
		record.value = reader.attributes().value(propertyValue).toInt();
		reader.readNextStartElement();
		record.gamer = reader.readElementText();
		reader.readNextStartElement();
		record.time = QDateTime::fromString(reader.readElementText(), timeFormat);
		reader.readNextStartElement();
		list.push_back(record);
	}
	file.close();
}
bool Records::insert(const int score){
	short store = MAX_RECORDS;
	for(auto it = list.begin(), end = list.end();; ++it){
		if(!store--) return true;
		if(it != end){
			if(score <= it->value) continue;
		}
		Record record;
		record.value = score;
		record.gamer = appConfig->getGamerName();
		record.time = QDateTime::currentDateTime();
		list.insert(it, record);
		break;
	}
	if(list.size() > MAX_RECORDS) list.pop_back();
	QSaveFile file(AppData::records());
	if(!file.open(QIODevice::WriteOnly)) return false;
	QXmlStreamWriter writer(&file);
	writer.setAutoFormatting(true);
	writer.writeStartDocument();
	writer.writeStartElement(tagHighscores);
	for(const Record &record: list){
		writer.writeStartElement(tagHighscore);
		writer.writeAttribute(propertyValue, QString::number(record.value));
		writer.writeTextElement(tagGamer, record.gamer);
		writer.writeTextElement(tagTime, record.time.toString(timeFormat));
		writer.writeEndElement();
	}
	writer.writeEndDocument();
	return file.commit();
}
QString Records::getWinnerInfo(int &score){
	if(list.empty()){
		score = 0;
		return QString();
	}
	auto winner = list.begin();
	score = winner->value;
	return winner->gamer;
}
void Records::fillTable(QTableWidget *table){
	QStringList header;
	header.append(QObject::tr("Score"));
	header.append(QObject::tr("Gamer's name"));
	header.append(QObject::tr("Date"));
	table->setHorizontalHeaderLabels(header);
	short index = 0;
	for(const Record &record: list){
		table->insertRow(index);
		table->setItem(index, 0, new QTableWidgetItem(QString::number(record.value)));
		table->setItem(index, 1, new QTableWidgetItem(record.gamer));
		table->setItem(index, 2, new QTableWidgetItem(record.time.toString(timeFormat)));
		index++;
	}
}
bool verifyFile(QXmlStreamReader *reader){
	if(!reader->readNextStartElement()) return false;
	if(reader->name() != tagHighscores) return false;
	return true;
}
