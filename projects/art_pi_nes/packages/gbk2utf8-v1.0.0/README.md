# gbk2utf8

#### 介绍
GBK及UTF8编码之间相互转换

#### 使用说明

1.  本软件包来源自https://github.com/duanlangtaosha/utf8_gbk.git
如有侵权，请告知并在此道歉
2.  使用例子：
`
	char *name = NULL; 
	utf82gbk(&name, (void *)gbk_name, strlen(gbk_name));
`
注意：name需要使用rt_free释放

