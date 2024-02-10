import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.12

Window
{
    id: mainWin
    visible: true
    width: 640
    height: 480
    title: qsTr( "Screen Shot Compare" )

    ColumnLayout
    {
        anchors.fill: parent
        spacing: 0

        ListView
        {
            id : mListView
            model : scrShotModel
            delegate: Rectangle
            {
                width : mainWin.width
                height: 100
                color: "beige"
                border.color: "yellowgreen"
                radius: 5

                Image
                {
                    id: snapShot
                    height: parent.height - 4
                    fillMode: Image.PreserveAspectFit
                    clip:true
                    anchors
                    {
                        left: parent.left
                        leftMargin: 2
                        top: parent.top
                        topMargin: 2
                    }
                    source: imageData ? "image://imageProvider" : ""
                    cache: false
                }

                Text
                {
                    id : textTime
                    anchors
                    {
                        left: snapShot.right
                        leftMargin: 10
                        top: parent.top
                        topMargin: ( parent.height - font.pointSize ) / 2
                    }
                    text : timeData
                    font.pointSize: 12
                }

                Text
                {
                    id : textMatch
                    anchors
                    {
                        left: textTime.right
                        leftMargin: 10
                        top: parent.top
                        topMargin: ( parent.height - font.pointSize ) / 2
                    }
                    text : matchData.toFixed( 2 ) + "%"
                    font.pointSize: 12
                }

                Text
                {
                    id : textHash
                    anchors
                    {
                        left: textTime.right
                        leftMargin: 70
                        top: parent.top
                        topMargin: ( parent.height - font.pointSize ) / 2
                    }
                    text : hashData
                    font.pointSize: 12
                }
            }
            Layout.fillHeight: true
            Layout.fillWidth: true
        }

        Button
        {
            id : startButton
            text : "Start timer"
            Layout.fillWidth: true
            onClicked:
            {
                scrShotDataSource.startTimer()
            }

        }
        Button
        {
            id : stopButton
            text : "Stop timer"
            Layout.fillWidth: true
            onClicked:
            {
                scrShotDataSource.stopTimer()
            }
        }
    }
}
