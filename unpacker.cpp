#include <QMessageBox>
#include <QString>
#include <archive_entry.h>
#include "unpacker.h"
#define BLOCK_SIZE 4096
Unpacker::Unpacker(const QString &fileName, FilterFunction filter):
	entry(nullptr), data(nullptr), capacity(0){
	arch = archive_read_new();
	if(fileName.isEmpty()){
		QMessageBox::critical(Q_NULLPTR, QObject::tr("Balls in Rows"), QObject::tr("Cannot find some of the required data files.\nReinstall the application."));
		return;
	}
	archive_read_support_format_tar(arch);
	filter(arch);
	archive_read_open_filename(arch, fileName.toUtf8().constData(), BLOCK_SIZE);
}
Unpacker::~Unpacker(){
	delete[] data;
	archive_read_free(arch);
}
const uchar *Unpacker::readNextEntryData(uint &length){
	if(archive_read_next_header(arch, &entry)) return nullptr;
	length = uint(archive_entry_size(entry));
	if(length > capacity){
		delete[] data;
		data = new uchar[length];
	}
	archive_read_data(arch, data, length);
	return data;
}
