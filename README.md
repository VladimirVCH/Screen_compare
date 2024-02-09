# Screen_compare
Screen compare GUI tool Qt/QML/SQLite

Description

App makes a screenshot once a minute. Start/Stop is controlled by the GUI button. Each new image is compared to previous and % of match is calculated as well as hash.
Image comparison is done in a separate thread. Image and details are available in the grid view GUI.
DB SQLite is used to keep all images and attributes.
Each new image is available on top of the grid.
Cross-platform ( Linux / Windows )

Qt 5
QML
С++11
SQLite
