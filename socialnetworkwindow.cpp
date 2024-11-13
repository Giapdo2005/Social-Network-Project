#include "socialnetworkwindow.h"
#include "ui_socialnetworkwindow.h"
#include "post.h"
#include "user.h"
#include "network.h"


SocialNetworkWindow::SocialNetworkWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::SocialNetworkWindow)
{
    ui->setupUi(this);


    char* filename = "users.txt";
    char* fileName2 = "posts.txt";
    myNetwork.readUsers(filename);
    myNetwork.readPosts(fileName2);
    ui->friends_profile->setVisible(false);
    ui->users_profile->setVisible(false);
    ui->friendsList->setVisible(false);
    ui->users_posts->setVisible(false);
    ui->users_posts->setWordWrap(true);
    ui->user_label->setVisible(false);
    ui->backtoUser->setVisible(false);
    ui->suggestedFriends->setVisible(false);
    ui->addFriend->setVisible(false);
    ui->suggestedFriends_2->setVisible(false);
    ui->addPost->setVisible(false);
    ui->postLabel->setVisible(false);
    ui->postInput->setVisible(false);

    connect(ui->loginButton,
            &QPushButton::clicked,
            this,
            &SocialNetworkWindow::userLoginCheck);

    connect(ui->friendsList,
            &QTableWidget::cellClicked,
            this,
            &SocialNetworkWindow::friendClicked);

    connect(ui->backtoUser,
            &QPushButton::clicked,
            this,
            &SocialNetworkWindow::backToUserClicked);

    connect(ui->suggestedFriends,
            &QTableWidget::cellClicked,
            this,
            &SocialNetworkWindow::suggestedFriendClicked);


    connect(ui->addFriend,
            &QPushButton::clicked,
            this,
            &SocialNetworkWindow::addSuggestedFriend);

    connect(ui->addPost,
            &QPushButton::clicked,
            this,
            &SocialNetworkWindow::addPost);
}


void SocialNetworkWindow::userLoginCheck() {
    QString fullname = ui->nameInput->toPlainText();
    std::string fullnameStd = fullname.toStdString();

    if (myNetwork.getId(fullnameStd) != -1) {
        int userId = myNetwork.getId(fullnameStd);

        loggedInUser = myNetwork.getUser(userId);

        if (loggedInUser) {
            std::set<int> friends = loggedInUser->getFriends();

            ui->nameInput->setVisible(false);
            ui->loginButton->setVisible(false);
            ui->nameLabel->setVisible(false);
            ui->users_profile->setVisible(true);
            ui->friendsList->setVisible(true);
            ui->users_posts->setVisible(true);
            ui->user_label->setVisible(true);
            ui->addFriend->setVisible(false);
            ui->suggestedFriends_2->setVisible(true);
            ui->addPost->setVisible(true);
            ui->postLabel->setVisible(true);
            ui->postInput->setVisible(true);



            ui->friendsList->setColumnCount(1);
            ui->friendsList->setColumnWidth(0, 150);
            ui->friendsList->setRowCount(friends.size());

            refreshPostsTable();

            int i = 0;
            for (int friendId : friends) {
                User* friendUser = myNetwork.getUser(friendId);


                if (friendUser) {
                    std::string friendName = friendUser->getName();

                    QTableWidgetItem* item = new QTableWidgetItem(QString::fromStdString(friendName));

                    ui->friendsList->setItem(i,0, item);
                    i++;
                }

            }

            int score;
            std::vector<int> suggestedFriends = myNetwork.suggestFriends(userId, score);
            qDebug() << "Calling suggestFriends for user ID:" << userId;
            ui->suggestedFriends->setVisible(true);

            ui->suggestedFriends->setColumnCount(1);
            ui->suggestedFriends->setColumnWidth(0, 150);
            ui->suggestedFriends->setRowCount(suggestedFriends.size());

            i = 0;
            for ( int sfriend : suggestedFriends) {
                qDebug() << "Suggested Friend ID:" << sfriend;
                User* friendUser = myNetwork.getUser(sfriend);


                if (friendUser) {
                    std::string friendName = friendUser->getName();

                    QTableWidgetItem* item = new QTableWidgetItem(QString::fromStdString(friendName));

                    ui->suggestedFriends->setItem(i,0, item);

                    i++;
                }

            }

        } else {
            std::cout << "User not found" << std::endl;
        }

    } else {
        ui->nameLabel->setText("User not found. Please enter an exisiting user.");
    }
}


void SocialNetworkWindow::addPost() {
    QString newPost = ui->postInput->toPlainText();
    std::string newPostStd = newPost.toStdString();

    if (!newPostStd.empty()) {
        int userId = loggedInUser->getId();

        myNetwork.addPost(userId, newPostStd, 0, false, "", true);

        myNetwork.writePosts("posts.txt");
        refreshPostsTable();
    } else {
        ui->postLabel->setText("Post can't be empty! Please enter your post.");
    }
    ui->postInput->setText("");
}


void SocialNetworkWindow::deletePost(int row){
    int userId = loggedInUser->getId();
    int messageIndex = loggedInUser->getSize() - 1 - row;
    myNetwork.deletePost(userId, messageIndex);

    myNetwork.writePosts("posts.txt");
    refreshPostsTable();
}

void SocialNetworkWindow::refreshPostsTable() {
    if (!loggedInUser) {
        qDebug() << "No logged-in user to load posts for.";
        return;
    }

    qDebug() << "Refreshing posts table for user:" << QString::fromStdString(loggedInUser->getName());

    ui->users_posts->clearContents();

    std::vector<std::string> posts = myNetwork.getPostsString(loggedInUser->getId(), loggedInUser->getSize(), false);
    qDebug() << "Number of posts retrieved:" << posts.size();

    ui->users_posts->setRowCount(posts.size());
    ui->users_posts->setColumnCount(2);
    ui->users_posts->setColumnWidth(0, 570);

    for (std::size_t i = 0; i < posts.size(); ++i) {
        QString postContent = QString::fromStdString(posts[i]);
        qDebug() << "Adding post at row" << i << ":" << postContent;

        QTableWidgetItem* item = new QTableWidgetItem(postContent);
        ui->users_posts->setItem(i, 0, item);
        ui->users_posts->setRowHeight(i, 100);

        QPushButton* button = new QPushButton("Delete Post", ui->users_posts);
        ui->users_posts->setCellWidget(i, 1, button);

        connect(button, &QPushButton::clicked, this, [=]() {
            deletePost(i);
        });
    }
}


void SocialNetworkWindow::friendClicked(int row, int column) {
    QTableWidgetItem* user = ui->friendsList->item(row, column);

    if (user) {
        QString userName = user->text();
        std::string fullname = userName.toStdString();
        qDebug() << "Clicked on user:" << userName;

        int userId = myNetwork.getId(fullname);
        currentUser = myNetwork.getUser(userId);
        std::vector<std::string> posts = myNetwork.getPostsString(userId, 5, false);
        if (!posts.empty()) {
            if (currentUser == loggedInUser) {
                for ( int i = 0; i < posts.size(); i++) {
                    std::string post = posts[i];

                    QTableWidgetItem* item = new QTableWidgetItem(QString::fromStdString(post));

                    ui->users_posts->setItem(i, 0, item);
                }
            } else {
                for ( int i = 0; i < posts.size(); i++) {
                    ui->users_posts->setColumnCount(1);
                    std::string post = posts[i];

                    QTableWidgetItem* item = new QTableWidgetItem(QString::fromStdString(post));

                    ui->users_posts->setItem(i, 0, item);

                }
            }
        }
        else {
            std::cerr << "No posts" << std::endl;
        }



        if (loggedInUser) {
            std::set<int> myfriends = loggedInUser->getFriends();

            if (myfriends.find(userId) == myfriends.end()) {
                ui->addFriend->setVisible(true);
            } else {
                ui->addFriend->setVisible(false);
            }
        }

        if (currentUser) {
            std::set<int> friends = currentUser->getFriends();

            ui->friends_profile->setVisible(true);
            ui->friends_profile->setText(userName);
            ui->users_profile->setVisible(false);
            ui->backtoUser->setVisible(true);
            ui->addPost->setVisible(false);
            ui->postLabel->setVisible(false);
            ui->postInput->setVisible(false);



            int i = 0;
            for (int friendId : friends) {
                User* friendUser = myNetwork.getUser(friendId);


                if (friendUser) {
                    std::string friendName = friendUser->getName();

                    QTableWidgetItem* item = new QTableWidgetItem(QString::fromStdString(friendName));

                    ui->friendsList->setItem(i,0, item);
                    i++;
                }
            }
            int score;
            i = 0;
            std::vector<int> suggestedFriends = myNetwork.suggestFriends(userId, score);
            for ( int sfriend : suggestedFriends) {
                qDebug() << "Suggested Friend ID:" << sfriend;
                User* friendUser = myNetwork.getUser(sfriend);


                if (friendUser) {
                    std::string friendName = friendUser->getName();

                    QTableWidgetItem* item = new QTableWidgetItem(QString::fromStdString(friendName));

                    ui->suggestedFriends->setItem(i,0, item);

                    i++;
                }

            }
        }

    }
}

void SocialNetworkWindow::backToUserClicked() {
    ui->users_profile->setVisible(true);
    ui->friends_profile->setVisible(false);

    if (loggedInUser) {
        std::set<int> friends = loggedInUser->getFriends();
        std::string fullname = loggedInUser->getName();


        ui->friends_profile->setVisible(false);
        ui->users_profile->setVisible(true);
        ui->backtoUser->setVisible(true);
        ui->user_label->setVisible(true);
        ui->user_label->setText("My post");
        ui->addFriend->setVisible(false);
        ui->backtoUser->setVisible(false);
        ui->addPost->setVisible(true);
        ui->postLabel->setVisible(true);
        ui->postInput->setVisible(true);

        std::vector<std::string> posts = myNetwork.getPostsString(loggedInUser->getId(), 5, false);

        if (!posts.empty()) {
            for ( int i = 0; i < posts.size(); i++) {
                ui->users_posts->setColumnCount(2);
                std::string post = posts[i];

                QTableWidgetItem* item = new QTableWidgetItem(QString::fromStdString(post));

                ui->users_posts->setItem(i, 0, item);

                QPushButton* button = new QPushButton("Delete Post", ui->users_posts);
                ui->users_posts->setCellWidget(i, 1, button);
            }
        }
        else {
            std::cerr << "No posts" << std::endl;
        }



        int i = 0;
        for (int friendId : friends) {
            User* friendUser = myNetwork.getUser(friendId);


            if (friendUser) {
                std::string friendName = friendUser->getName();

                QTableWidgetItem* item = new QTableWidgetItem(QString::fromStdString(friendName));

                ui->friendsList->setItem(i,0, item);
                i++;
            }
        }
        int score;
        i = 0;
        std::vector<int> suggestedFriends = myNetwork.suggestFriends(loggedInUser->getId(), score);
        for ( int sfriend : suggestedFriends) {
            qDebug() << "Suggested Friend ID:" << sfriend;
            User* friendUser = myNetwork.getUser(sfriend);


            if (friendUser) {
                std::string friendName = friendUser->getName();

                QTableWidgetItem* item = new QTableWidgetItem(QString::fromStdString(friendName));

                ui->suggestedFriends->setItem(i,0, item);
                i++;
            }

        }
    }

}


void SocialNetworkWindow::suggestedFriendClicked(int row, int column) {
    QTableWidgetItem* user = ui->suggestedFriends->item(row, column);
    ui->addFriend->setVisible(true);

    if (user) {
        QString userName = user->text();
        std::string fullname = userName.toStdString();
        qDebug() << "Clicked on user:" << userName;

        int userId = myNetwork.getId(fullname);
        std::vector<std::string> posts = myNetwork.getPostsString(userId, 5, false);
        if (!posts.empty()) {
            for ( int i = 0; i < posts.size(); i++) {
                ui->users_posts->setColumnCount(1);
                std::string post = posts[i];

                QTableWidgetItem* item = new QTableWidgetItem(QString::fromStdString(post));

                ui->users_posts->setItem(i, 0, item);
            }
        }
        else {
            std::cerr << "No posts" << std::endl;
        }

        currentUser = myNetwork.getUser(userId);

        if (currentUser) {
            std::set<int> friends = currentUser->getFriends();

            ui->friends_profile->setVisible(true);
            ui->friends_profile->setText(userName);
            ui->users_profile->setVisible(false);
            ui->backtoUser->setVisible(true);
            ui->addPost->setVisible(false);
            ui->postLabel->setVisible(false);
            ui->postInput->setVisible(false);


            int i = 0;
            for (int friendId : friends) {
                User* friendUser = myNetwork.getUser(friendId);


                if (friendUser) {
                    std::string friendName = friendUser->getName();

                    QTableWidgetItem* item = new QTableWidgetItem(QString::fromStdString(friendName));

                    ui->friendsList->setItem(i,0, item);
                    i++;
                }
            }
            int score;
            i = 0;
            std::vector<int> suggestedFriends = myNetwork.suggestFriends(userId, score);
            for ( int sfriend : suggestedFriends) {
                qDebug() << "Suggested Friend ID:" << sfriend;
                User* friendUser = myNetwork.getUser(sfriend);


                if (friendUser) {
                    std::string friendName = friendUser->getName();

                    QTableWidgetItem* item = new QTableWidgetItem(QString::fromStdString(friendName));

                    ui->suggestedFriends->setItem(i,0, item);

                    i++;
                }

            }
        }

    }
}

void SocialNetworkWindow::addSuggestedFriend() {
    char* filename = "users.txt";

    int loggedInUserId = loggedInUser->getId();
    int userId = currentUser->getId();
    loggedInUser->addFriend(userId);
    currentUser->addFriend(loggedInUserId);
    myNetwork.writeUsers(filename);

}


SocialNetworkWindow::~SocialNetworkWindow()
{
    delete ui;
}

