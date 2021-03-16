#include "csvIO.h"

CSVReader::CSVReader(const std::filesystem::path& name)
{
	file = new QFile(name);
	if (!file->open(QIODevice::ReadOnly | QIODevice::Text))
	{
		throw std::runtime_error("Open csv file failed!");
	}
	ts = new QTextStream(file);
}

CSVReader::~CSVReader()
{
	delete ts;
	file->close();
	delete file;
}

//PatternHeader GameInfo::CSVReader::GetHeader()
//{
//	using namespace std;
//	vector<long long> headerData = ReadLongLongRow();
//	PatternHeader header =
//	{
//		headerData[0],
//		headerData[1],
//		headerData[2],
//	};
//	return header;
//}

QStringList CSVReader::ReadRow()
{
	QString line;
	line = ts->readLine();
	QStringList strList = line.split(",", Qt::SkipEmptyParts);
	return strList;
}

std::vector<long long> CSVReader::ReadLongLongRow()
{
	QStringList strings = ReadRow();
	std::vector<long long> longs;
	for (auto& str : strings)
	{
		longs.push_back(str.toLongLong());
	}
	return longs;
}

std::vector<int> CSVReader::ReadIntRow()
{
	QStringList strings = ReadRow();
	std::vector<int> ints;
	for (auto& str : strings)
	{
		ints.push_back(str.toInt());
	}
	return ints;
}

bool CSVReader::AtEnd()
{
	return ts->atEnd();
}

void CSVReader::DiscardRow()
{
	ts->readLine();
}
