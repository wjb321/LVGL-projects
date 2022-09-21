#include "QuickSort.h"


int quickSort(int s[], int l, int r)
{ 
	int medianVal = 0;
	medianVal = (int)(r+1)/2.0;
	if (l< r)
	{      
		int i = l, j = r, x = s[l];
		while (i < j)
		{
			while(i < j && s[j]>= x) // ���������ҵ�һ��С��x����
				j--; 
			if(i < j)
				s[i++] = s[j];
			while(i < j && s[i]< x) // ���������ҵ�һ�����ڵ���x����
				i++; 
			if(i < j)
				s[j--] = s[i];
		}
		s[i] = x;
		quickSort(s, l, i - 1); // �ݹ����
		quickSort(s, i + 1, r);
	}
	return s[medianVal]; 
}