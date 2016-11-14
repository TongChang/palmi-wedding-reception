/**
* @note 本プログラムの最終行にある変数宣言は非常に重要です。
* SPCフレームワークは、「spcApp」と言う固定の変数に対して処理を行うように作られています。
*/

#include "spc/spcbase2.h"
#include "SpcBaseCode.h"

void doWelcomeSpeech()
{
	// 文字を読み始める
	while (true) {
		speak(setting.welcomeSpeech);
	}
}

/**
* @brief アプリケーション初期化イベント。
*
* アプリケーションの実行開始時に呼び出されます。
*/
void palmi_wedding_reception::onInitialize()
{
	setLogLevel(SPC_LOG_LEVEL_TRACE);
	setActionPOTFluctuation(SPC_ONLY_NOTYFY_POT_FLUCTUATION);
	SPC_LOG_INFO("onInitialize");

	// SDカードを読む
	if (mountMicroSD() != 0) {
		speak("おやおや、Micro SDカードが取り外されているみたいですよっ。");
		exitComponent();
		return;
	}
	try {
		std::string sdDir;
		getMicroSDDirPath(sdDir);
		std::string configFilePath = sdDir + "/config.txt";
		setting.load(configFilePath);
	}
	catch (Poco::Exception& ex) {
		SPC_LOG_ERROR(ex.displayText().c_str());
		throw;
	}
	catch (...) {
		SPC_LOG_ERROR("error load config.");
		throw;
	}

	speak("準備オッケーです。");
	SPC_LOG_INFO("onInitialize end");

	doWelcomeSpeech();

	// アプリケーションを終了する場合は、以下の関数「exitComponent()」を呼び出してください。
	exitComponent();
}

/**
* @brief 顔認識イベント
*
* Palmiが顔を見つけた場合に呼び出されます。
*
* @param [in] name 認識した個人の登録名(全角カタカナ)。個人未確定の場合は空白文字列。
*/
void palmi_wedding_reception::onFaceCatch(std::string name)
{
}


/**
* @brief 顔認識イベント
*
* Palmiが顔を見失った場合に呼び出されます。
*/
void palmi_wedding_reception::onFaceDrop()
{
}


/**
* @brief POTセンサーイベント
*
* Palmiの頭部をさわった時に呼び出されます。
*/
void palmi_wedding_reception::onPOTFluctuationCatch()
{
	// 写真用のファイルパスを生成
	std::string sdDir;
	getMicroSDDirPath(sdDir);
	Poco::DateTime dateTime;
	std::string dateTimeString = Poco::DateTimeFormatter::format(dateTime, "%Y%m%d%H%M%S");
	std::string picFileName = "pic-" + dateTimeString + ".jpg";
	std::string picFilePath = sdDir + "/" + picFileName;
	SPC_LOG_INFO("picFilePath: %s", picFilePath.c_str());

	// お客さん
	speak("あっ、お客さんですね。");

	speak("本日は、圭さんと文さんの結婚式に、ようこそいらっしゃいました。")
	speak("わたくし、受付カメラマン 兼 案内役のPalmiと申します。よろしくお願いいたします。");

	speak("さっそくですが、");

	// 写真を撮る
	// XXX 拒否されたら言葉を変える
	long takeResult = takePicture(SPC_TAKE_PICTURE_JPG_1600, SPC_TAKE_PICTURE_ACTION_NORMAL, picFilePath);

	speak("ありがとうございます。");
	speak("素敵な写真がとれました。お二人に渡しておきますね。");

	// 披露宴会場の案内
	spc::SPC_SYNCMOTION_PARAM syncMotionParam;

	speak("会場は");

	// モーション同時の発話内容
	syncMotionParam.type = SPC_SYNCMOTION_SPEAK;
	syncMotionParam.param = "ここから右手まっすぐ行ったところにあります、 あらたま のお部屋でございます。";

	// 道案内モーション
	startMotion(sdDir + "/guidance.rec", syncMotionParam);

	// おしまい
	speak("あ、お二人から、メッセージがあるんです。");
	speak("短い時間ですが、ごゆっくりお楽しみくださいね。");
	speak("とのことです。");

	speak("それでは、会場にお進みください。");
}


/**
* @brief タイマーイベント
*
* startTimer関数で設定した時間が経過した場合に呼び出されます。
*
* @param [in] timerId startTimer関数で取得したタイマーID
*/
void palmi_wedding_reception::onTimer(long timerId)
{
}

/**
* @brief アプリケーション終了イベント。
*
* アプリケーションの終了時に呼び出されます。
*/
void palmi_wedding_reception::onFinalize() {
}



// SPCアプリケーションインスタンス生成
palmi_wedding_reception	spcApp;

