#include "qword.h"
#include <QApplication>
#include <QtCore>
#include <QMessageBox>


int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
//	QTextCodec *codec = QTextCodec::codecForName("GB2312");
//	QTextCodec::setCodecForLocale(codec);
//	QTextCodec::setCodecForCStrings(codec);
//	QTextCodec::setCodecForTr(codec);

	QWord word;
	word.createNewWord();				
//	word.setPageOrientation(1);			//页面横向
//	word.setWordPageView(3);			//页面视图
////	word.insertMoveDown();				//插入回车
    word.setFontSize(25);				//字体大小
    word.setParagraphAlignment(0);		//下面文字置中
    word.setFontBold(true);				//字体加粗
    word.insertText("多维智能远程巡检系统");//插入文字
    word.setFontBold(false);			//字体加粗
    word.insertMoveDown();
    word.setParagraphAlignment(1);		//下面文字置左
    word.setFontSize(14);
    word.insertText(QString("巡检单位：智能变电站辅助系统综合监控平台"));//插入文字
    word.insertMoveDown();
//	word.insertText(QString("巡检计划：220kV 小坝变"));//插入文字
//	word.insertMoveDown();
//	word.insertText(QString("巡检时间：2014-04-21 10:19:41"));//插入文字
//	word.insertMoveDown();
//	word.insertText(QString("巡 检 人："));//插入文字
//	word.insertMoveDown();
//	word.setFontSize(12);
//	word.setParagraphAlignment(0);		//下面文字置中
//	word.insertText(QString("巡检结果统计表"));
//	word.insertMoveDown();
//	word.setParagraphAlignment(0);		//下面文字置
////	word.insertText(QString("序号    巡检点  巡检时间  类型  巡检图片  数据  参考  人工判断  备注"));
//	word.intsertTable(4,9);			//几行几列table
////	word.openWord();
//	word.setColumnWidth(1,40);
//	word.setColumnWidth(2,70);
//	word.setColumnWidth(3,80);
//	word.setColumnWidth(4,50);
//	word.setColumnWidth(5,200);
//	word.setColumnWidth(6,70);
//	word.setColumnWidth(7,50);
//	word.setColumnWidth(8,70);
//	word.setCellString(1,1,"序号");
//	word.setCellString(1,2,"巡检点");
//	word.setCellString(1,3,"巡检时间");
//	word.setCellString(1,4,"类型");
//	word.setCellString(1,5,"巡检图片");
//	word.setCellString(1,6,"数据");
//	word.setCellString(1,7,"参考");
//	word.setCellString(1,8,"人工判断");
//	word.setCellString(1,9,"备注");
//	word.setCellString(2,1,"1");
//	word.setCellString(2,2,"周界防区1");
//	word.setCellString(2,3,"2014-04-21\n10:19:41");
//	word.setCellString(2,4,"报警");
////	word.setCellString(2,5,"无");
//	word.insertCellPic(2,5,"D:\\temp\\223-223.jpg");
//	word.setCellString(2,6,"报警开始");
//	word.setCellString(2,7,"参考");
//	word.setCellString(2,8,"□正常\n\n□异常");
//	word.setCellString(2,9," ");
//	word.setCellString(3,1,"2");
//	word.setCellString(3,2,"周界防区1");
//	word.setCellString(3,3,"2014-04-21\n10:19:41");
//	word.setCellString(3,4,"报警");
//	//word.setCellString(3,5,"无");
//	word.insertCellPic(3,5,"D:\\temp\\123.jpg");
//	word.setCellString(3,6,"报警开始");
//	word.setCellString(3,7,"参考");
//	word.setCellString(3,8,"□正常\n\n□异常");
//	word.setCellString(3,9," ");
//	word.setCellString(4,1,"3");
//	word.setCellString(4,2,"周界防区1");
//	word.setCellString(4,3,"2014-04-21\n10:19:41");
//	word.setCellString(4,4,"报警");
////	word.setCellString(4,5,"无");
//	word.insertCellPic(4,5,"D:\\temp\\225-225.jpg");
//	word.setCellString(4,6,"报警开始");
//	word.setCellString(4,7,"参考");
//	word.setCellString(4,8,"□正常\n\n□异常");
//	word.setCellString(4,9," ");
//	word.deleteSelectColumn(1);
//	word.setTableAutoFitBehavior(4);
//	word.moveForEnd();
//	word.insertMoveDown();
//	word.insertMoveDown();
//	word.setParagraphAlignment(2);		//下面文字置右
//	word.setFontSize(14);
//	word.insertText("巡检人签字：_______________");
//	word.insertMoveDown();
//	word.insertText("报表时间：2014-04-22 10:06:38");
	
	word.close();
    cout << "word close.";
    return a.exec();
}
