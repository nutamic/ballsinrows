#ifndef UNPACKER_H
#define UNPACKER_H
#include <QByteArray>
#include <archive.h>
#include "resources.h"
class Unpacker{
	typedef int (*FilterFunction)(struct archive *);
	archive *arch;
	archive_entry *entry;
	uchar *data;
	uint capacity;
public:
	Unpacker(const QString &fileName, FilterFunction filter);
	~Unpacker();
	const uchar *readNextEntryData(uint &length);
	QByteArray readNextEntryData(){
		uint length;
		readNextEntryData(length);
		return QByteArray(const_str(data), int(length));
	}
};
#endif
