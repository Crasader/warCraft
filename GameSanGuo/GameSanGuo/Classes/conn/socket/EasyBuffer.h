#ifndef __EASY_BUFFER_H__
#define __EASY_BUFFER_H__

#include <assert.h>
#include <string.h>

/**
 * @brief 用于接收和发送的缓冲区。
 * @author bymm
 */
class EasyBuffer
{
//function
public:
	/* ctor */
	EasyBuffer(unsigned int size) : _size(size)
	{
		_buffer = new char[size];
		reset();
	}

	/* dtor */
	~EasyBuffer()
	{
		delete[] _buffer;
	}

	/* 获得区块WRITE空间。 */
	char* getFreeWriteBuffer(unsigned int& len)
	{
		if (_wPos > _rPos)
			len = _size - _wPos;
		else if (_wPos < _rPos)
			len = _rPos - _wPos;
		else //equal
			len = _isFull ? 0 : _size;
		return _buffer + _wPos;
	}

	/* 获得区块READ空间。 */
	char* getFreeReadBuffer(unsigned int& len)
	{
		if (_wPos > _rPos)
			len = _wPos - _rPos;
		else if (_wPos < _rPos)
			len = _size - _rPos + _wPos;
		else //equal
			len = _isFull ? _size : 0;
		return _buffer + _rPos;
	}

	/* 移动区块WRITE位置，这个在写入buffer操作之后调用。 */
	void addWritePos(unsigned int len)
	{
		addPos(_wPos, len);
		if (_wPos == _rPos)
			_isFull = true;
	}

	/* 移动区块READ位置，保持一致。 */
	void addReadPos(unsigned int len)
	{
		addPos(_rPos, len);
		if (_rPos == _wPos)
			reset();
	}

	/* 从buffer中读取n个字节，并写入dst中。 */
	bool readBuffer(char* dst, unsigned int n)
	{
		//不允许大于缓冲区总容量。
		assert(n < _size);
		//不读取OR内容区不够，返回。
		if ( n <= 0 || n > getAvailableReadSpaece() )
			return false;
		//开始读取
		if (_wPos > _rPos) //单段可读
		{
			memcpy(dst, _buffer + _rPos, n);
		}
		else if (_wPos <= _rPos) //多段可读
		{
			unsigned int rightReadSize = _size - _rPos;
			if (n <= rightReadSize) //右填。
				memcpy(dst, _buffer + _rPos, n);
			else
			{
				unsigned int leftReadSize = n - rightReadSize;
				memcpy(dst, _buffer + _rPos, rightReadSize); //右填。
				memcpy(dst + rightReadSize, _buffer, leftReadSize); //左填。
			}
		}
		addReadPos(n);
		return true;
	}

	//HACK: 本API暂时专为发送消息而设计，功能并不完整，请不要在别处使用。
	/* 从src中读取n个字节，并写入buffer中。 */
	bool writeBuffer(void* src, unsigned int n)
	{
		//不允许大于缓冲区总容量。
		if (n <= 0 || n > _size)
			return false;
		memcpy(_buffer + _wPos, src, n);
		addWritePos(n);
	}

	/* 获得读取位置。 */
	unsigned int getReadPos()
	{
		return _rPos;
	}

	/* 获得写入位置。 */
	unsigned int getWritePos()
	{
		return _wPos;
	}

	/* 重置读写。 */
	void reset()
	{
		_wPos = _rPos = 0;
		_isFull = false;
        memset(_buffer, 0, _size);
	}

private:
	/* 增加pos的计数。 */
	void addPos(unsigned int& pos, unsigned int n)
	{
		pos += n;
		if (pos >= _size)
			pos -= _size;
	}

	/* 获得可以读取的buffer大小。 */
	unsigned int getAvailableReadSpaece()
	{
		unsigned int availRead = 0;
		if (_wPos > _rPos)
			availRead = _wPos - _rPos;
		else if (_wPos < _rPos)
			availRead = (_size - _rPos) + _wPos;
		else //equal
			availRead = _isFull ? _size : 0;
		return availRead;
	}

//variable
private:
	/* 当前读取缓冲指针位置。 */
	unsigned int _rPos;

	/* 当前写入缓冲指针位置。 */
	unsigned int _wPos;

	/* 当前缓冲区块指针。 */
	char* _buffer;

	/* 缓冲区块总大小。 */
	unsigned int _size;

	/* 缓冲区是否满载。 */
	bool _isFull;
};


#endif //__EASY_BUFFER_H__
