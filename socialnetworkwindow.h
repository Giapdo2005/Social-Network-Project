#ifndef SOCIALNETWORKWINDOW_H
#define SOCIALNETWORKWINDOW_H

#include <QMainWindow>
#include "network.h"


QT_BEGIN_NAMESPACE
namespace Ui {
class SocialNetworkWindow;
}
QT_END_NAMESPACE

class SocialNetworkWindow : public QMainWindow
{
    Q_OBJECT

public:
    SocialNetworkWindow(QWidget *parent = nullptr);
    ~SocialNetworkWindow();

    void userLoginCheck();
    void friendClicked(int row, int column);
    void backToUserClicked();
    void addSuggestedFriend();
    void suggestedFriendClicked(int row, int column);
    void deletePost(int row);
    void addPost();
    void refreshPostsTable();
private:
    Ui::SocialNetworkWindow *ui;
    Network myNetwork;
    User *loggedInUser;
    User *currentUser;
};
#endif // SOCIALNETWORKWINDOW_H

