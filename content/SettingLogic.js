.pragma library
.import QtQuick.LocalStorage 2.0 as Sql
.import QtMultimedia 5.8 as Media

var refDpi = 216;
var refWidth = 480;
var refHeight = 600;

var gamePath = "";
var gameLevel = null;

var ballSrc = "qrc:/Ball.qml";
var ballComp = null;
var balls = [];

var wallSrc = "qrc:/Wall.qml";
var wallComp = null;
var wallObjs = [];

var Sc = null;
var scaleW = 0.0;
var scaleH = 0.0;

var polyCom = null;

var startX = 0; //记录第一次交汇时
var startY = 0;

var x1 = 0;
var y1 = 0;

var levelPath = "qrc:/levels/Level";
var isFirstRun = 0;
var grade = 1;  //当前关卡
var level = 1;
var highestGrade = 1;//最高关卡
var highestLevel = 1;


var isMouseClicked = false;
var isDraw = false;
var isLineDrawn = false;
var lineComp = null;
var lineTimer = null;

var db = null;
var lockgrades = [true,true,true,true,true,true];
var locklevels = [true,true,true,true,true,true,true,true,true];

var sound_switch = true;
var music_switch = true;

function applyWScale(Screen,refValue) {
    Sc = Screen;
    var scaleRatio = 1;
    if(Qt.platform.os == "android")
    {
        var realW = Screen.desktopAvailableWidth;
        scaleRatio = realW / refWidth;
    }
    scaleW = scaleRatio;
    return scaleRatio * refValue;
}

function applyHScale(Screen,refValue) {
    Sc = Screen;
    var scaleRatio = 1;
    if(Qt.platform.os == "android")
    {
        var realH = Screen.desktopAvailableHeight;
        scaleRatio = realH / refHeight;
    }
    scaleH = scaleRatio;
    return scaleRatio * refValue;
}

function gradeLevelDecrease()
{
    if(level == 1)
    {
        level = 9;
        grade -= 1;
    }
    else
    {
        level -= 1;
    }
}

function gradeLevelIncrease()
{
    if(level == 9)
    {
        level = 1;
        grade += 1;
    }
    else
    {
        level += 1;
    }

    highestGrade = grade;
    highestLevel = level;

    updateGradeLevel();
}

function addLineTimer(gameZone,progressBar)
{
    if(lineTimer == null)
    {
       lineTimer = Qt.createQmlObject("import QtQuick 2.0; Timer {}", gameZone);
       lineTimer.interval = 50;
       lineTimer.repeat = true;
       lineTimer.triggeredOnStart = true;
       lineTimer.triggered.connect(function () {
           var ballsPos = [];
           var ballsRadius = [];
           for(var i = 0;i < balls.length;i++)
           {
               ballsPos.push(Qt.point(balls[i].x,balls[i].y));
               ballsRadius.push(balls[i].radius);
           }

           if(lineComp == null)
               return;

           var vRet = polyCom.isBallCrossLine(lineComp.p1.x,lineComp.p1.y,lineComp.p2.x,lineComp.p2.y,ballsPos,ballsRadius);
           if(vRet == 1)
           {
               gameZone._playSound(4);
               lineTimer.interval = 999999;    //设置为999999代表现在不要画线，为了省一个控制变量
               lineTimer.stop();
               ballsSlowDown();
               isDraw = false;
               isLineDrawn = false;
               isMouseClicked = false;
               if(lineComp != null)
               {
                   lineComp.destroy(0);
                   lineComp = null;
               }
               delay(gameZone,progressBar);
           }
       })
    }
    lineTimer.start();
}

function initGameZone(gameZone,url)
{
    loadLevel(gameZone,url);
    addBall(gameZone);
    addPolygon(gameZone);
    initWall(gameZone);
}

function deInitGameZone(gameZone,progressBar)
{
    //0.进度条归零
    progressBar.value = 0;
    //1.清除所有的球
    for(var i = 0;i < balls.length;i++)
        balls[i].destroy(0);
    balls = [];
    //2.清除所有的墙
    for(var i = 0;i < wallObjs.length;i++)
        wallObjs[i].destroy(10);
    wallObjs = [];

    polyCom.deInit();
    polyCom.destroy(10);

    if(lineComp !== null)
    {
        lineComp.destroy(0);
        lineComp = null;
    }
    isMouseClicked = false;
    isDraw = false;
    isLineDrawn = false;
}

function addWall(gameZone,pts,type)
{
    if(gameLevel == null)
        return;

    if(wallComp == null)
    {
        wallComp = Qt.createComponent(wallSrc)
        if(wallComp.status != 1)
        {
            console.log("wallComp error: " + wallComp.errorString());
            return;
        }
    }

    var tempScaleW = scaleW;
    var tempScaleH = scaleH;
    if(type != 0){
        tempScaleW = 1;
        tempScaleH = 1;
    }
    for(var i = 0;i < pts.length;i+=2)
    {
        var ret = [];
        ret.push(Qt.point(pts[i]*tempScaleW,pts[i+1]*tempScaleH));
        if(i == pts.length-2)
            ret.push(Qt.point(pts[0]*tempScaleW,pts[1]*tempScaleH));
        else
            ret.push(Qt.point(pts[i+2]*tempScaleW,pts[i+3]*tempScaleH));

        var wallObj = wallComp.createObject(gameZone,
              {"vertices":ret,
               "world": gameZone.physicsWorld
              })
        if (wallObj == null)
        {
            console.log("error creating block");
            console.log(ballComp.errorString());
            return false;
        }
        wallObjs.push(wallObj);
    }
}

function updateWall(gameZone,polygon)
{
    for(var i = 0;i < wallObjs.length;i++)
        wallObjs[i].destroy(10);
    wallObjs = [];
    addWall(gameZone,polygon,1);
}

function isDrawLine(backGround,winDialog,gameZone,progressBar,x2,y2)
{
    if(polyCom == null)
        return;

    if(lineTimer != null && lineTimer.interval == 999999)
        return;

    if(!isMouseClicked)
        return;

    if(!isDraw)
    {
        //isDraw = polyCom.isShouldDrawLine(startX,startY,x2,y2);
        var temp = polyCom.isCrossPolygon(x1,y1,x2,y2);
        if(temp == 1)
        {
            isDraw = true;
        }
        else if(temp == 9999)
        {
            isMouseClicked = false;
            gameZone._playSound(3);
            gameZone._playSparkAnim(x2,y2);
        }
        if(!isDraw)
            return;
    }

    if(isDraw)
    {
        if(!isLineDrawn)
        {
            var pt1 = polyCom.getLineStart();
            startX = pt1.x;
            startY = pt1.y;
            var str = 'import QtQuick 2.6;import LL.BLine 1.0;BLine {id: line; p1: Qt.point(';
            str += pt1.x+','+pt1.y+');p2: Qt.point(';
            str += x2+','+y2+');}'

            lineComp = Qt.createQmlObject(str,gameZone,"dynamicSnippet1");
            if(lineComp == null)
            {
                console.log("createQmlObject error");
                return;
            }
            isLineDrawn = true;
            addLineTimer(gameZone,progressBar);
        }
        else
        {
            lineComp.p2 = Qt.point(x2,y2);
            var temp = polyCom.isCrossPolygon(x1,y1,x2,y2);
            if(temp == 2)
            {
                x1 = x2;
                y1 = y2;
                resetLine();
                dealSlashPoly(backGround,winDialog,progressBar,gameZone,x2,y2);
            }
            else if(temp == 9999)
            {
                x1 = x2;
                y1 = y2;
                resetLine();
                gameZone._playSound(3);
                gameZone._playSparkAnim(x2,y2);
            }
        }
    }
}

function ballsSlowDown()
{
    for(var i = 0;i < balls.length;i++)
    {
        balls[i].body.linearVelocity = (Qt.point(0,0));
        balls[i].color = "gray";
    }
}

function delay(gameZone,progressBar)
{
    var timer = Qt.createQmlObject("import QtQuick 2.0; Timer {}", gameZone);
    timer.interval = 3000;
    timer.repeat = false;
    timer.triggeredOnStart = false;
    timer.triggered.connect(function () {
        console.log("I'm triggered once every second");
        lineTimer.interval = 50;

        deInitGameZone(gameZone,progressBar);
        var temp = levelPath + grade + level + ".qml";
        initGameZone(gameZone,temp);

        timer.destroy(0);
    })
    timer.start();
}

function dealSlashPoly(backGround,winDialog,progressBar,gameZone,x2,y2)
{
    //处理画线切割多边形
    //遍历当前球获取位置
    var ballsPos = [];
    var ballsRadius = [];
    for(var i = 0;i < balls.length;i++)
    {
        ballsPos.push(Qt.point(balls[i].x,balls[i].y));
        ballsRadius.push(balls[i].radius);
    }

    var ret = polyCom.calcSlashPoly(refWidth * scaleW,refHeight * scaleH,startX,startY,x2,y2,ballsPos,ballsRadius);
    if(ret == 2)
    {
        gameZone._playSound(2);
        //添加一条闪烁的line
        var str = 'import QtQuick 2.6;import LL.BLine 1.0;BLine {id: line; p1: Qt.point(';
        str += startX+','+startY+');p2: Qt.point(';
        str += x2+','+y2+');'
        str += 'SequentialAnimation on opacity { loops: 3;PropertyAnimation { to: 0 } PropertyAnimation { to: 1 } }}';

        var tempCom = Qt.createQmlObject(str,gameZone,"dynamicSnippet1");
        if(tempCom == null)
        {
            console.log("createQmlObject error");
            return;
        }
        tempCom.destroy(1000);
    }
    else if(ret == 0)
    {
        gameZone._playSound(1);
        var pts = [];
        pts = polyCom.getResultPoly(0);
        updateWall(gameZone,pts);
        var progress = polyCom.getProgress();
        progressBar.value = progress;
        if(progress > 0.8)
        {

            backGround.source = "qrc:/pics/beautys/" + grade + level + ".jpg";
            winDialog.open();
            gradeLevelIncrease();
        }
    }
    else if(ret == 1)
    {
        gameZone._playSound(4);

        ballsSlowDown();
        if(lineComp != null)
        {
            lineComp.destroy(0);
            lineComp = null;
        }
        delay(gameZone,progressBar);
    }
}

function resetLine()
{
    if(lineComp != null)
    {
        lineComp.destroy(0);
        isDraw = false;
        isLineDrawn = false;
        lineComp = null;
    }
}

function isCutPolygon()
{
    if(polyCom == null)
        return;

    if(lineTimer != null && lineTimer.interval == 999999)
        return;

    resetLine();
}

function initWall(gameZone)
{
    if(gameLevel == null)
        return;
    addWall(gameZone,gameLevel.wallData,0);
}

function addSound(gameZone)
{
    console.log("addSound");
    var str = 'import QtMultimedia 5.8; SoundEffect{ id: slash;source: "qrc:/sound/slash.wav";}';
    sound_splash = Qt.createQmlObject(str,gameZone,"dynamicSnippet1");

    str = 'import QtMultimedia 5.8; SoundEffect{ id: slash_fail;source: "qrc:/sound/slash_fail.wav";}'
    sound_splash_fail = Qt.createQmlObject(str,gameZone,"dynamicSnippet1");

    str = 'import QtMultimedia 5.8; SoundEffect{ id: slash_steel; source: "qrc:/sound/steel.wav";}'
    sound_splash_steel = Qt.createQmlObject(str,gameZone,"dynamicSnippet1");
}

function delSound(gameZone)
{
    if(sound_splash != null)
    {
        sound_splash.destroy(0);
        sound_splash = null;
    }
    if(sound_splash_fail != null)
    {
        sound_splash_fail.destroy(0);
        sound_splash_fail = null;
    }
    if(sound_splash_steel != null)
    {
        sound_splash_steel.destroy(0);
        sound_splash_steel = null;
    }
}

function addPolygon(gameZone)
{
    if(gameLevel == null)
        return false;

    var str = 'import LL.BPolygon 1.0;BPolygon{id: polygon;points:[';
    for (var idx = 0,i = 0; idx < gameLevel.wallData.length; idx+=2,i++) {
        str += 'Qt.point(';
        str += gameLevel.wallData[idx]*scaleW;
        str += ',';
        str += gameLevel.wallData[idx+1]*scaleH;
        str += '),';
        str += 'Qt.point(';
        str += gameLevel.wallRigid[i];
        str += ',0),';
    }

    str += ']}';

    polyCom = Qt.createQmlObject(str,gameZone,"dynamicSnippet1");
    if(polyCom == null)
    {
        console.log("createQmlObject error");
        return false;
    }

    return true;
}

function addBall(gameZone)
{
    if(gameLevel == null)
        return;

    var colors = [];
    colors.push("yellow");
    colors.push("blue");
    colors.push("red");
    colors.push("purple");
    colors.push("orange ");
    colors.push("green");
    colors.push("brown");

    ballComp = Qt.createComponent(ballSrc);
    if(ballComp.status != 1)
    {
        console.log("ballComp error: " + ballComp.errorString());
        return;
    }

    //球基本数据-x,y位置、半径、密度、摩擦力、恢复系数、速度（x、y）
    var posX;var posY;var radius;var density;
    var speedx; var speedy;

    for (var i = 0; i < gameLevel.ballData.length;i++)
    {
        posX = gameLevel.ballData[i][0]*scaleW;
        posY = gameLevel.ballData[i][1]*scaleH;
        radius = gameLevel.ballData[i][2]*scaleW;
        density = gameLevel.ballData[i][3];
        speedx = gameLevel.ballData[i][6];
        speedy = gameLevel.ballData[i][7];

        if(ballComp.status == 1)
        {
            var dynamicObject = ballComp.createObject(gameZone,
                  {"x": posX,
                   "y": posY,
                   "width": radius * 2,
                   "height": radius * 2,
                   "world": gameZone.physicsWorld,
                   "z": 1,
                   "color":colors[i%7],
                   "visible": true
                  })

            if (dynamicObject == null)
            {
                console.log("error creating block");
                console.log(ballComp.errorString());
                return false;
            }

            //dynamicObject.body.applyLinearImpulse(Qt.point(10,10),dynamicObject.body.getWorldCenter());
            dynamicObject.body.linearVelocity = (Qt.point(speedx,speedy));
            //dynamicObject.body.linearVelocity = (Qt.point(0,0));
            balls.push(dynamicObject);
        }
    }
}

function loadLevel(gameZone,url)
{
    gamePath = url;
    var levelComp = Qt.createComponent(gamePath);
    if(levelComp.status != 1){
        console.log("Error loading level " + levelComp.errorString());
        //提示错误
        return;
    }
    gameLevel = levelComp.createObject(gameZone);
    if(gameLevel == null)
    {
        //提示错误
        console.log("Error Create levelComp:" + levelComp.errorString());
        return;
    }
}

function setLockGrades()
{
    for(var i = 0;i < highestGrade;i++)
        lockgrades[i] = false;
}

function isDatabaseCreated(db)
{
    isFirstRun = 1;
    db.changeVersion("", "1.0");
    db.transaction(
                function(tx){
                    tx.executeSql('CREATE TABLE [settings] ([id] INTEGER  NOT NULL PRIMARY KEY AUTOINCREMENT,[music] BOOLEAN DEFAULT \'true\' NULL,[sound] BOOLEAN DEFAULT \'true\' NULL,[curGrade] INTEGER DEFAULT \'1\' NULL,[curLevel] Integer DEFAULT \'1\' NULL)');
                    tx.executeSql('INSERT INTO settings VALUES(?,?,?,?,?)',[0,true,true,1,1]);
                }
          )
}

function initDB()
{
    db = Sql.LocalStorage.openDatabaseSync("BeautySlash","1.0","BeautySlash local storage",10000,isDatabaseCreated);
    db.transaction(
            function(tx){
                var rs = tx.executeSql('SELECT * FROM settings');
                if(rs.rows.length > 0)
                {
                    highestGrade = rs.rows.item(0).curGrade;
                    highestLevel = rs.rows.item(0).curLevel;
                    if(rs.rows.item(0).sound === "true" || rs.rows.item(0).sound === 1)
                        sound_switch = true;
                    else
                        sound_switch = false;
                    if(rs.rows.item(0).music === "true" || rs.rows.item(0).music === 1)
                        music_switch = true;
                    else
                        music_switch = false;
                    //highestGrade = 6;
                    //highestLevel = 9;
                    grade = highestGrade;
                    level = highestLevel;
                }
            }
        )
}

function updateGradeLevel()
{
    db.transaction(
                function(tx){
                    tx.executeSql('update settings Set curGrade ='+highestGrade+',curLevel='+highestLevel+" where id=0");
                }

                )
}

function updateSoundSetting()
{
    db.transaction(
                function(tx){
                    tx.executeSql('update settings Set music =\''+music_switch+'\',sound=\''+sound_switch+"\' where id=0");
                }
                )
}
