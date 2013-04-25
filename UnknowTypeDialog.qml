import QtQuick 2.0
import Ubuntu.Components 0.1
import Ubuntu.Components.Popups 0.1

Dialog {
    title: "Unknow type"
    text: "Sorry but we can't find a way to display this file. Do you want to open it as a plain text ?"
    Button {
        text: "Yes"
        color: "green"
    }
    Button {
        text: "No"
        color: "red"
    }
}

