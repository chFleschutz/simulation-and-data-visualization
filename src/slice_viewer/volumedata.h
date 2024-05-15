#pragma once

#include <vector>
#include <string>
#include <unordered_map>
#include <fstream>
#include <filesystem>

#include <QFile>
#include <QDataStream>
#include <QTextStream>
#include <QString>

struct VolumeData
{
	std::vector<int> dimensions;
	std::vector<uint16_t> data;
};

class VolumeFileLoader
{
public:
	static void load(const std::filesystem::path& filename, VolumeData& data)
	{
		auto metadata = loadMhd(filename);
		checkMhd(metadata);

		// Get the dimensions
		auto dimSizes = metadata["DimSize"].split(' ');
		size_t elementCount = 1;
		for (auto& size : dimSizes)
		{
			int dimSize = size.toInt();
			elementCount *= dimSize;
			data.dimensions.push_back(dimSize);
		}

		std::string rawFilename = filename.parent_path().string() + "/" + metadata["ElementDataFile"].toStdString();
		if (metadata["ElementType"] == "MET_UCHAR")
		{
			loadRaw<unsigned char>(rawFilename, data, elementCount);
		}
		else if (metadata["ElementType"] == "MET_SHORT")
		{
			loadRaw<short>(rawFilename, data, elementCount);
		}
		else if (metadata["ElementType"] == "MET_USHORT")
		{
			loadRaw<unsigned short>(rawFilename, data, elementCount);
		}
		else if (metadata["ElementType"] == "MET_INT")
		{
			loadRaw<int>(rawFilename, data, elementCount);
		}
		else if (metadata["ElementType"] == "MET_UINT")
		{
			loadRaw<unsigned int>(rawFilename, data, elementCount);
		}
		else if (metadata["ElementType"] == "MET_FLOAT")
		{
			loadRaw<float>(rawFilename, data, elementCount);
		}
		else if (metadata["ElementType"] == "MET_DOUBLE")
		{
			loadRaw<double>(rawFilename, data, elementCount);
		}
		else
		{
			throw std::runtime_error("Unsupported element type");
		}
	}

	static std::unordered_map<QString, QString> loadMhd(const std::filesystem::path& filename)
	{
		QFile file(filename);
		if (!file.open(QIODevice::ReadOnly))
		{
			throw std::runtime_error("Could not open file: " + filename.string());
		}

		std::unordered_map<QString, QString> metadata;
		QTextStream in(&file);
		while (!in.atEnd())
		{
			auto line = in.readLine().trimmed();
			int separator = line.indexOf('=');

			if (separator == -1)
				continue;

			auto key = line.left(separator).trimmed();
			auto value = line.mid(separator + 1).trimmed();

			metadata[key] = value;
		}
		file.close();
		return metadata;
	}

	static void checkMhd(const std::unordered_map<QString, QString>& metadata)
	{
		if (!metadata.contains("NDims"))
			throw std::runtime_error("Invalid header file: Missing NDims");

		if (!metadata.contains("DimSize"))
			throw std::runtime_error("Invalid header file: Missing DimSize");

		if (!metadata.contains("ElementDataFile"))
			throw std::runtime_error("Invalid header file: Missing ElementDataFile");
	}

	template<typename T>
	static void loadRaw(const std::filesystem::path& filename, VolumeData& data, int elementCount)
	{
		QFile rawFile(filename);
		if (!rawFile.open(QIODevice::ReadOnly))
		{
			throw std::runtime_error("Could not open file: " + filename.string());
		}

		QDataStream stream(&rawFile);
		stream.setByteOrder(QDataStream::LittleEndian);

		data.data.resize(elementCount);
		for (int i = 0; i < elementCount; i++)
		{
			T value;
			stream >> value;
			data.data[i] = static_cast<uint16_t>(value);
		}
	}
};
