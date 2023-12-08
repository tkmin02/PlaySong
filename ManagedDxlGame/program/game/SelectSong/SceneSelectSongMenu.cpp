#include "DxLib.h"
#include "../dxlib_ext/dxlib_ext.h"
#include "../Manager/SceneManager.h"
#include "../Title/SceneTitle.h"
#include "../Main.h"
#include "../SelectSong/SceneSelectSongMenu.h"
#include "../Play/ScenePlaySong.h"


int genreCount[GENRE_NUM];


const char* SelectSongMenu::_songTitle[PLAYLIST_NUM] =
{
	"�����ȑ�9��",
	"BadApple!!",
	"���\���o�[�Y" ,
	"�c���ȓV�g�̃e�[�["
};

const char* SelectSongMenu::_songGenre[PLAYLIST_NUM] =
{
	"�N���V�b�N",
	"����Vocal",
	"�{�[�J���C�h",
	"�A�j�\��"
};


const char* SelectSongMenu::_songGenreRefAll[GENRE_NUM] =
{
	"�N���V�b�N",
	"J-POP",
	"�A�j�\��",
	"����Vocal",
	"�{�[�J���C�h",
	"���b�N",
	"�Q�[���~���[�W�b�N"
};


const char* SelectSongMenu::_songLevels[4] =
{
	"EASY",
	"BASIC",
	"HARD",
	"ABYSS"
};


void SelectSongMenu::SongGenreTotalCount() {

	// �ǉ��ς݃v���C���X�g
	for (int i = 0; i < PLAYLIST_NUM; i++)
	{
		// �S�ȃW������
		for (int j = 0; j < GENRE_NUM; j++)
		{
			// �W������������v������J�E���g�𑝂₷
			if (strcmp(_songGenre[i], _songGenreRefAll[j]) == 0)
			{
				genreCount[j]++;
				break; // ��v�����烋�[�v�𔲂���
			}
		}
	}
}



void SelectSongMenu::Render_TotalSongGenreList() {


	// �W�������ƋȐ���\��  �����c����
	for (int i = 0; i < GENRE_NUM; i++)
	{
		if (show_finalCheck_before_startPlaySong) SetDrawBlendMode(DX_BLENDMODE_ALPHA, dimScreen_alphaSize);

		if (strcmp(	 // �I�𒆂̋Ȃ̃W�������Ȃ�F��ς���
			_songGenre[songIndex],
			_songGenreRefAll[i]) == 0)
		{
			SetDrawBright(0, 255, 0); // �F��΂ɂ���
		}

		// �W�������ƋȐ���\������
		DrawFormatString(50, 20 + i * 33, -1, "%s�F%d", _songGenreRefAll[i], genreCount[i]);
		SetDrawBright(255, 255, 255); // �F�𔒂ɖ߂�
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);
	}
}



void SelectSongMenu::SelectingSongByInput() {

	if (songSelect) {

		// ��L�[
		if (tnl::Input::IsKeyDownTrigger(eKeys::KB_UP) || tnl::Input::IsPadDownTrigger(ePad::KEY_UP))
		{
			StopSoundMem(songList[songIndex]);
			// �Đ��ʒu�������ʒu�ɖ߂�
			SetCurrentPositionSoundMem(0, songList[songIndex]);

			songIndex--;
			if (songIndex < 0) songIndex = PLAYLIST_NUM - 1;
		}
		// ���L�[
		if (tnl::Input::IsKeyDownTrigger(eKeys::KB_DOWN) || tnl::Input::IsPadDownTrigger(ePad::KEY_DOWN))
		{
			StopSoundMem(songList[songIndex]);
			// �Đ��ʒu�������ʒu�ɖ߂�
			SetCurrentPositionSoundMem(0, songList[songIndex]);

			songIndex++;
			if (songIndex >= PLAYLIST_NUM) songIndex = 0;
		}
	}
}



void SelectSongMenu::PickOneSongByInput() {

	// �I�𒆂̋ȃ��[�v�Đ�
	PlaySoundMem(songList[songIndex], DX_PLAYTYPE_LOOP, false);

	bool soundPlayed = false;

	// ����L�[�������ꂽ�ꍇ
	if (tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN) || tnl::Input::IsPadDownTrigger(ePad::KEY_1))
	{
		// �ȑI�𒆂Ȃ��Փx�I���Ɉڍs����
		if (songSelect) {
			levelSelect = true;
			songSelect = false;
		}
		// ��Փx�I�𒆂Ȃ�ŏI�m�F�������\��
		else if (levelSelect)
		{
			show_finalCheck_before_startPlaySong = true;
			levelSelect = false;
		}
		// �ŏI�m�F���Ȃ�v���C�V�[���ɑJ��
		else if (show_finalCheck_before_startPlaySong)
		{
			if (!soundPlayed) {
				PlaySoundMem(titleToSelectMenu_hdl, DX_PLAYTYPE_BACK, TRUE);
				soundPlayed = true;
			}
			moveToPlayScene = true;


			auto mgr = SceneManager::GetInstance();
			mgr->SceneChange(new PlaySong(_songTitle[songIndex],  //�I����
				_songLevels[levelIndex],  //�I���Փx�A
				songIndex, levelIndex)); // �I�񂾋Ȃ̔ԍ� �� PlaySong�V�[���ɓn��
		}
	}

	if (moveToPlayScene) soundPlayed = false;
}



void SelectSongMenu::BackToPreviousByInput() {


	// �L�����Z���L�[�������ꂽ�ꍇ
	if (tnl::Input::IsKeyDownTrigger(eKeys::KB_BACK) || tnl::Input::IsPadDownTrigger(ePad::KEY_0))
	{
		// ��Փx�I�𒆂Ȃ�ȑI���ɖ߂�
		if (levelSelect) {

			levelIndex = 0;
			levelSelect = false;
			songSelect = true;
		}
		else if (show_finalCheck_before_startPlaySong) {

			// �ŏI�m�F��ʂ�\�����Ȃ��Փx�I���ɖ߂�
			show_finalCheck_before_startPlaySong = false;
			levelSelect = true;
		}
	}
}



void SelectSongMenu::SelectingLevelByInput() {


	if (levelSelect) {

		// ��Փx�I�𒆂Ȃ��Փx�̃C���f�b�N�X�����炷
		if (tnl::Input::IsKeyDownTrigger(eKeys::KB_LEFT) || tnl::Input::IsPadDownTrigger(ePad::KEY_LEFT))
		{
			levelIndex--;
			if (levelIndex < 0)
				levelIndex = _LEVEL_NUM - 1;
		}

		if (tnl::Input::IsKeyDownTrigger(eKeys::KB_RIGHT) || tnl::Input::IsPadDownTrigger(ePad::KEY_RIGHT))
		{
			levelIndex++;
			if (levelIndex >= _LEVEL_NUM)
				levelIndex = 0;
		}
	}
}


void SelectSongMenu::StartPlaySongByInput() {


	if (show_finalCheck_before_startPlaySong) {

		SetDrawBlendMode(DX_BLENDMODE_ALPHA, dimScreen_alphaSize);

		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);

		SetFontSize(100);
		DrawStringEx(_SELECTED_SONG_TEXT_POS_X, _SELECTED_SONG_TEXT_POS_Y, -1,
			_songTitle[songIndex]);   // �I�������Ȗ�

		SetFontSize(50);

		DrawStringEx(_SELECTED_LEVEL_TEXT_POS_X, _SELECTED_LEVEL_TEXT_POS_Y, -1,
			_songLevels[levelIndex]); // �I��������Փx

		DrawStringEx(_START_TEXT_POS_X, _STARTBACK_TEXT_POS_Y, -1, "Start");
		DrawStringEx(_BACK_TEXT_POS_X, _STARTBACK_TEXT_POS_Y, -1, "Back");

		SetDrawBlendMode(DX_BLENDMODE_ALPHA, dimScreen_alphaSize);
	}
	else {
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}
}


void SelectSongMenu::RenderAndChangeColor_SongTitle() {


	for (int i = 0; i < PLAYLIST_NUM; i++)
	{
		// �I�𒆂̋ȂȂ�F��ς���
		if (songSelect && i == songIndex)
		{
			SetDrawBright(255, 255, 0); // �F�����F�ɂ���

		}
		else {
			SetDrawBright(255, 255, 255); // �F�𔒂ɖ߂�
		}
		// �Ȗ���\������
		DrawStringEx(_PLAYLIST_X_POS, _PLAYLIST_Y_POS * (i + 1), -1, _songTitle[i]);
	}
}


void SelectSongMenu::RenderAndChangeColor_SongLevel() {

	for (int i = 0; i < _LEVEL_NUM; i++)
	{
		// �I�𒆂̓�Փx�Ȃ�F��ς���				
		if (levelSelect && i == levelIndex)
		{
			SetDrawBright(255, 255, 0); // �F�����F�ɂ���
		}
		else {
			SetDrawBright(255, 255, 255); // �F�𔒂ɖ߂�
		}

		// ��Փx��\������
		DrawOvalAA(335 + ((i + 1) * 100), _LEVELLIST_Y_POS, 50, 50, 8, -1, false, 1);
		SetFontSize(15);
		DrawStringEx(_LEVELLIST_X_POS + ((i + 1) * 100), _LEVELLIST_Y_POS, -1, _songLevels[i]);
	}
}


void SelectSongMenu::RenderBigSizeTitle_AtCenter() {


	SetDrawBright(255, 255, 255);
	//��Փx��ɑI�𒆂̋Ȃ̃^�C�g���i��j
	SetFontSize(80);
	DrawFormatString(_BIGSIZE_TITLE_POS_X, _BIGSIZE_TITLE_POS_Y, -1, _songTitle[songIndex]);

	SetFontSize(40);
	DrawStringEx(1070, 640, -1, "Enter");
	DrawStringEx(10, DXE_WINDOW_HEIGHT - 45, -1, "music select...");
}


void SelectSongMenu::Render() {

	// �v���C�V�[���ɂ܂����ł��Ȃ���Ε`��
	if (moveToPlayScene) return;

	Render_TotalSongGenreList();

	SelectingSongByInput();

	PickOneSongByInput();

	BackToPreviousByInput();

	SelectingLevelByInput();

	StartPlaySongByInput();

	RenderAndChangeColor_SongTitle();

	RenderAndChangeColor_SongLevel();

	RenderBigSizeTitle_AtCenter();
}


void SelectSongMenu::Update(float deltaTime) {

	sequence.update(deltaTime);

	if (moveToPlayScene) {

		sequence.update(deltaTime);

		StopSoundMem(songList[songIndex]);
		SetCurrentPositionSoundMem(0, songList[songIndex]);
		songSelect = true;
	}
}

// �v���C�V�[���J�ڊ֐�
bool SelectSongMenu::SeqIdle(float deltaTime) {

	if (moveToPlayScene) {

	}

	// �^�C�g���V�[���֖߂鏈�������ƂŒǉ�

	return true;
}