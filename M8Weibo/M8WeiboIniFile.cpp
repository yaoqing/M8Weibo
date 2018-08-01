#include "M8WeiboIniFile.h"

CM8WeiboIniFile::CM8WeiboIniFile(wchar_t *pFileName)
{
	first = NULL;
	int len = wcslen(pFileName);
	m_pFileName = new wchar_t[len+1];
	wsprintf(m_pFileName, L"%s", pFileName);

	// 读取文件大小，如果文件不存在则新建
	FILE *fd = _wfopen(m_pFileName, L"r+");
	if (fd == NULL)
	{
		fd = _wfopen(m_pFileName, L"w+");
	}
	fseek(fd, 0, SEEK_END);
	long size = ftell(fd);
	fclose(fd);

	// 读取文件内容
	char *buffer = new char[size+1];
	memset(buffer, 0, size+1);
	fd = _wfopen(m_pFileName, L"r");
	long count = 0;
	while (count < size && !feof(fd))
	{
		 count += fread(buffer+count,  sizeof(char), size, fd);
	}
	buffer[size] = '\n';
	fclose(fd);

	// 初始化链表
	tagIniLineStruct *temp, *last = first;
	long i = 0, j = 0;
	char *prev,  *next;
	prev = next = buffer;
	while ((next = strchr(prev, '\n'))  != NULL)
	{
		*next = '\0';
		TrimLRSpace(prev, next);
		if (*prev != '#') TrimMidSpace(prev, next);
		char *fragment = new char[next-prev+1];
		memset(fragment, 0, next-prev+1);
		strcpy(fragment, prev);

		temp = new tagIniLineStruct();
		temp->buffer = fragment;
		if (last == NULL)
		{
			first = last = temp;
		}
		else
		{
			last->next = temp;
			last = temp;
		}
		if (*prev != '#') last->valid = TRUE;

		prev = next+1;
	}

	// 释放内存
	delete [] buffer;
}

CM8WeiboIniFile::~CM8WeiboIniFile(void)
{
	delete [] m_pFileName;

	// 删除链表
	tagIniLineStruct *temp, *iter = first;
	while (iter)
	{
		temp = iter;
		iter = iter->next;
		delete [] temp->buffer;
		delete temp;
	}
}

void CM8WeiboIniFile::TrimLRSpace(char * first, char * last)
{
	// 去除右侧空白字符
	char * p = last-1;
	while (isspace(*p) && p >= first)
	{
		*p-- = '\0';
	}

	// 去除左侧空白字符
	long i = 0, j = 0;
	while (i < last-first)
	{
		if (isspace(*(first+i)))
			j++;
		else
			break;
		i++;
	}
	if (j > 0)
	{
		for (i = 0; i < last-first-j; i++)
			*(first+i) = *(first+i+j);
		*(first+i) = '\0';
	}
}

void CM8WeiboIniFile::TrimMidSpace(char *first, char *last)
{
	// 去除中间空白字符
	long i = 0, j = 0;
	char *p = new char[last-first];
	for (; i < last -first; i++)
	{
		if (!isspace(first[i]))
		{
			p[j++] = first[i];
		}
	}
	p[j] = '\0';
	strcpy(first, p);
	delete [] p;
}

void CM8WeiboIniFile::saveToFile()
{
	tagIniLineStruct *iter = first;
	FILE *fd = _wfopen(m_pFileName, L"w+");
	while (iter)
	{
		fwrite(iter->buffer, sizeof(char), strlen(iter->buffer), fd);
		fwrite("\n", sizeof(char), 1, fd);
		iter = iter->next;
	}
	fclose(fd);
}

// 获取key对应的值，未找到返回NULL
char *CM8WeiboIniFile::getKey(char *key)
{
	int key_len = strlen(key);
	tagIniLineStruct *iter = first;
	while (iter)
	{
		if (iter->valid)
		{
			char *buffer = new char[strlen(iter->buffer)+1];
			sprintf(buffer, "%s=", key);
			if (strncmp(iter->buffer, buffer, key_len+1) == 0)
			{
				sprintf(buffer, "%s", iter->buffer+key_len+1);
				return buffer;
			}
			delete [] buffer;
		}
		iter = iter->next;
	}
	return NULL;
}

// 设置key和value
bool CM8WeiboIniFile::setKey(char *key, char *value)
{
	int key_len = strlen(key);
	int value_len = strlen(value);
	tagIniLineStruct *last, *iter = first;
	while (iter)
	{
		if (iter->valid)
		{
			char *buffer = new char[key_len+value_len+1+1];
			sprintf(buffer, "%s=", key);
			if (strncmp(iter->buffer, buffer, key_len+1) == 0)
			{
				sprintf(buffer, "%s=%s", key, value);
				delete [] iter->buffer;
				iter->buffer = buffer;
				return true;
			}
			delete [] buffer;
		}
		last = iter;
		iter = iter->next;
	}

	char *buffer = new char[key_len+value_len+1+1];
	sprintf(buffer, "%s=%s", key, value);
	tagIniLineStruct *temp = new tagIniLineStruct();
	temp->buffer = buffer;
	temp->valid = TRUE;
	last->next = temp;
	return true;
}
