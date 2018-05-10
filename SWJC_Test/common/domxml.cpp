#include "domXml.h"





DomXml::DomXml()
{

}

void DomXml::creatXML(QString filePath)
{
    QFile file(filePath);
    if(file.exists())
    {
        printLog(LOG_ERROR, "The file exist already!");
        return;
    }
    else
    {
        bool isOk = file.open(QIODevice::WriteOnly);
        if(true == isOk)
        {
            QDomDocument doc;
            //创建头部格式
            QDomProcessingInstruction ins;
            ins = doc.createProcessingInstruction("xml", "version=\"1.0\" encoding=\"UTF-8\"");

            //追加元素
            doc.appendChild(ins);

            //根节点元素
            QDomElement root = doc.createElement("sys_user");
            doc.appendChild(root);


            //passwd cnp200@QT
            QStringList list;
            list << "0" << "admin" << "08ae377132fcb0025b87dc42199ab84c" << "1" << "0" << "0" << "0" << "0";
            writeUserToXML(doc, root, list);

            //保存
            QTextStream stream(&file);  //文本流关联文件
            doc.save(stream, 4);

            file.close();
        }
        else
        {
            printLog(LOG_ERROR, "Failed to open file!, filePath:%s.", filePath);
            return;
        }
    }
}

void DomXml::appendXML(QString filePath, QStringList list)
{
    QFile file(filePath);
    bool isOk = file.open(QIODevice::ReadOnly);
    if(true == isOk)
    {
        //file doc关联
        QDomDocument doc;
        isOk = doc.setContent(&file);
        if (isOk)
        {
            file.close();

            //获取根节点元素
            QDomElement root = doc.documentElement();

            //判断根节点下有没有子节点
            if(root.hasChildNodes())
            {
                writeUserToXML(doc, root, list);

            }
            else
            {
                QStringList list;
                list << "0" << "admin" << "08ae377132fcb0025b87dc42199ab84c" << "1" << "0" << "0" << "0" << "0";
                writeUserToXML(doc, root, list);

//                //创建子节点元素
//                QDomElement child1 = doc.createElement("user");
//                //创建子节点属性
//                QDomAttr child1Attr = doc.createAttribute("id");
//                //设置属性值
//                child1Attr.setNodeValue("1");

//                //关联节点和属性
//                child1.setAttributeNode(child1Attr);

//                //添加子节点到父节点
//                root.appendChild(child1);

            }

            if (true == file.open(QIODevice::WriteOnly))
            {
                QTextStream stream(&file);
                doc.save(stream, 4);

                file.close();
            }
        }
        else
        {
            printLog(LOG_ERROR, "Doc associated file failed. filePath:%s.", filePath);
            return;
        }
    }
}

//void DomXml::readXML(QString filePath)
//{

//    cout << "readXML...";

//    QFile file(filePath);
//    if ( file.open(QIODevice::ReadOnly) )
//    {
//        QDomDocument doc;
//        doc.setContent(&file);

//        file.close();

//        //返回根节点
//        QDomElement root = doc.documentElement();

//        //返回根节点的第一个子节点
//        QDomNode child = root.firstChild();

//        /*
//        <root>  //根元素
//            <child id="01">   //root元素的第一个子元素，“id”是其属性
//                <node1>Qt</node1>   //book元素的子元素，“Qt”是元素的文本
//                <node2>shiming</node2>   //book元素的子元素，title元素的兄弟元素
//            </child>  //结束标记名
//        </root>
//        */

//        while(false == child.isNull())
//        {
//            if (child.isElement())
//            {
//                QDomElement childElement = child.toElement();
//                //cout << childElement.tagName().toUtf8().data();

//                qDebug() << qPrintable(childElement.tagName())   //返回元素标记
//                << qPrintable(childElement.attribute("id"));  //返回元素id属性的值


//                QDomNodeList list = childElement.childNodes();

//                for(int i = 0; i < list.count(); ++i)
//                {
//                    QDomNode node = list.at(i);
//                    if (node.isElement())
//                    {
//                        cout << "";
//                    }
//                }
//            }

//            //下一个兄弟节点
//            child = child.nextSibling();
//        }


//    }
//    else
//    {
//        cout << "打开文件失败！";
//    }
//    return;
//}

void DomXml::writeUserToXML(QDomDocument &doc, QDomElement &parsent, QStringList &list)
{
    //创建子节点元素
    QDomElement child1 = doc.createElement("user");
    //创建子节点属性
    QDomAttr child1Attr = doc.createAttribute("id");
    //设置属性值
    child1Attr.setNodeValue(list.at(0));

    //关联节点和属性
    child1.setAttributeNode(child1Attr);

    //添加子节点到父节点
    parsent.appendChild(child1);

    QDomElement elmUserName = doc.createElement("username");
    QDomText textUserName = doc.createTextNode(list.at(1));
    elmUserName.appendChild(textUserName);
    child1.appendChild(elmUserName);

    QDomElement elmPassWD = doc.createElement("passwd");
    QDomText textPassWD = doc.createTextNode(list.at(2));
    elmPassWD.appendChild(textPassWD);
    child1.appendChild(elmPassWD);


    QDomElement elmAuthority = doc.createElement("authority");
    QDomText textAuthority = doc.createTextNode(list.at(3));
    elmAuthority.appendChild(textAuthority);
    child1.appendChild(elmAuthority);

    QDomElement elmReserve1 = doc.createElement("reserve1");
    QDomText textReserve1 = doc.createTextNode(list.at(4));
    elmReserve1.appendChild(textReserve1);
    child1.appendChild(elmReserve1);

    QDomElement elmReserve2 = doc.createElement("reserve2");
    QDomText textReserve2 = doc.createTextNode(list.at(5));
    elmReserve2.appendChild(textReserve2);
    child1.appendChild(elmReserve2);

    QDomElement elmReserve3 = doc.createElement("reserve3");
    QDomText textReserve3 = doc.createTextNode(list.at(6));
    elmReserve3.appendChild(textReserve3);
    child1.appendChild(elmReserve3);

    QDomElement elmReserve4 = doc.createElement("reserve4");
    QDomText textReserve4 = doc.createTextNode(list.at(7));
    elmReserve4.appendChild(textReserve4);
    child1.appendChild(elmReserve4);



}


void DomXml::readDataByUserNameFromXML(QString filePath, QStringList& list, QString user)
{
    QStringList list_tmp;

    QFile file(filePath);
    if ( file.open(QIODevice::ReadOnly) )
    {
        QDomDocument doc;
        doc.setContent(&file);

        file.close();

        //返回根节点
        QDomElement root = doc.documentElement();

        //返回根节点的第一个子节点
        QDomNode child = root.firstChild();

        if (!root.hasChildNodes())
        {
            printLog(LOG_ERROR, "System files have been corrupted, failePath:%s.", filePath);
            return;
        }
        /*
        <sys_user>  //根元素
            <user id="0">
                <username>admin1</username>
                <passwd>08ae377132fcb0025b87dc42199ab84c</passwd>
            </user>
        </root>
        */

        //获取子节点，数目为
        QDomNodeList list = root.childNodes();
        int count = list.count();

        for(int i=0; i < count; ++i)
        {
            QDomNode dom_node = list.item(i);
            QDomElement element = dom_node.toElement();

            //获取id值
            //QString id_2 = element.attribute("id");
//            cout << id_2;
            if (element.attribute("username") == user)
            {
                //获取子节点，数目为7，包括：username、passwd, authority, reserve1 ...
                QDomNodeList child_list = element.childNodes();
                int child_count = child_list.count();

                if ((filePath == g_userLoginStstemFilename && child_count != 7) ||
                    (filePath == g_userLoginMysqlFilename && child_count != 4))
                {
                    cout << "系统文件已被破坏！";
                }

                QDomNode child_dom_node = child_list.item(0);
                QDomElement child_element = child_dom_node.toElement();


                //if (child_element.text() == user)
                {
                    list_tmp.append(child_element.text());
                    for(int j = 1; j < child_count; ++j)
                    {
                        child_dom_node = child_list.item(j);
                        child_element = child_dom_node.toElement();
                        //QString child_tag_name = child_element.tagName();
                        QString child__tag_value = child_element.text();
                        //cout << child_tag_name << child__tag_value;
                        list_tmp.append(child__tag_value);
                    }
                }
            }
        }

    }
    else
    {
        cout << "open file error!";
    }
    list = list_tmp;
    return;
}
