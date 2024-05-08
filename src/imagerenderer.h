#pragma once

#include <QImage>
#include <QPainter>
#include <QMessageBox>

class ImageRenderer
{
public:
	ImageRenderer() = default;
	~ImageRenderer() = default;

	void load(const QString& path)
	{
		if (!m_originalImage.load(path))
		{
			QMessageBox::critical(nullptr, "Error", "Failed to load image");
			return;
		}

		m_image = m_originalImage;
	}

	void load(const QString& path, const QSize& size)
	{
		m_originalImage.load(path);
		m_image = m_originalImage.scaled(size, Qt::KeepAspectRatio);
	}

	void resize(const QSize& size)
	{
		m_image = m_originalImage.scaled(size, Qt::KeepAspectRatio);
	}

	void draw(QPainter& painter, int x = 0, int y = 0) const
	{
		painter.drawImage(0, 0, m_image);
	}

	QImage& image() { return m_image; }
	QImage& originalImage() { return m_originalImage; }

	QSize size() const { return m_image.size(); }
	QSize originalSize() const { return m_originalImage.size(); }

private:
	QImage m_originalImage;
	QImage m_image;
	QPainter m_painter;
};