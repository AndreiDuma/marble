// This file is part of the Marble Virtual Globe.
//
// This program is free software licensed under the GNU LGPL. You can
// find a copy of this license in LICENSE.txt in the top directory of
// the source code.
//
// Copyright 2013 Andrei Duma <andrei.duma.dorian@gmail.com>

import QtQuick 1.0
import QtMultimediaKit 1.1
import com.nokia.meego 1.0
import org.kde.edu.marble 0.11
import org.kde.edu.marble.qtcomponents 0.12

Page {
    id: cloudSyncPage

    tools: ToolBarLayout {
        MarbleToolIcon {
            iconSource: main.icon( "actions/go-previous-view", 48 );
            onClicked: pageStack.pop()
        }
    }

    Row {
        id: syncSetting
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.margins: 10

        Label {
            id: syncLabel
            width: 142
            text: "Sync:"
            anchors.verticalCenter: parent.verticalCenter
        }
        Switch {
            id: syncSwitch
            checked: settings.owncloudSync
            anchors.verticalCenter: parent.verticalCenter
            onCheckedChanged: settings.owncloudSync = checked
        }
    }
    Item {
        id: credentialsPane

        width: parent.width
        height: 200
        anchors.left: parent.left
        anchors.top: syncSetting.bottom
        anchors.right: parent.right
        anchors.margins: 10

        Column {
            id: labelsColumn
            width: 140
            spacing: 35
            anchors.top: parent.top
            anchors.topMargin: 12
            Label {
                text: "Server:"
            }
            Label {
                text: "Username:"
            }
            Label {
                text: "Password:"
            }
        }
        Column {
            anchors.left: labelsColumn.right
            anchors.right: parent.right
            spacing: 10
            TextField {
                text: settings.owncloudServer
                width: parent.width
                readOnly: !syncSwitch.checked
                onAccepted: settings.owncloudServer = text
            }
            TextField {
                text: settings.owncloudUsername
                width: parent.width
                readOnly: !syncSwitch.checked
                onAccepted: settings.owncloudUsername = text
            }
            TextField {
                text: settings.owncloudPassword
                echoMode: TextInput.Password
                width: parent.width
                readOnly: !syncSwitch.checked
                onAccepted: settings.owncloudPassword = text
            }
        }
    }
    Row {
        anchors.top: credentialsPane.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.margins: 10

        Label {
            y: 8
            width: 142
            text: "Bookmarks:"
        }
        Column {
            spacing: 10
            width: parent.width - 142
            Row {
                spacing: 10
                Switch {
                    id: bookmarksSwitch
                    checked: true
                    //checked: settings.owncloudSync
                    anchors.verticalCenter: parent.verticalCenter
                    //onCheckedChanged: settings.owncloudSync = checked
                }
                Label {
                    text: bookmarksSwitch.checked ? "Sync automatically" : "Sync off"
                    anchors.verticalCenter: parent.verticalCenter
                }
            }
           Button {
               width: parent.width
               text: "Sync now"
               onClicked: {
                   //// ?!
                   console.log("CLICKED!!!!")
               }
           }
        }
    }
}