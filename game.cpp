//
//  game.cpp
//  GHOST
//
//  Created by IDE KENTARO on 2019/07/02.
//  Copyright © 2019 IDEKENTARO. All rights reserved.
//

#include "game.hpp"
#include "del/delegate.hpp"
#include "mgr/mapmanager.hpp"
#include "mgr/fontmanager.hpp"
#include "mgr/menumanager.hpp"
#include "mgr/eventmanager.hpp"
#include "mgr/audiomanager.hpp"
#include "mgr/statusmanager.hpp"
#include "mgr/playermanager.hpp"
#include "mgr/endingmanager.hpp"
#include "mgr/controlmanager.hpp"
#include "mgr/openingmanager.hpp"
#include "mgr/collisionmanager.hpp"
#include "mgr/charactermanager.hpp"

Game::Game()
{
    
    // Map (Episode X Map Y)
    _map = "e1m1";
    
    // Initialization of MapManager
    _mapmanager       = new MapManager();
    // Initialization of CollisionManager
    _collisionmanager = new CollisionManager();
    // Initialization of PlayerManager
    _playermanager    = new PlayerManager();
    // Initialization of CharacterManager
    _charactermanager = new CharacterManager();
    // Initialization of ControlManager
    _controlmanager   = new ControlManager();
    // Initialization of AudioManager
    _audiomanager     = new AudioManager();
    // Initialization of FontManager
    _fontmanager      = new FontManager();
    // Initialization of EventManager
    _eventmanager     = new EventManager();
    // Initialization of StatusManager
    _statusmanager    = new StatusManager();
    // Initialization of MenuManager
    _menumanager      = new MenuManager();
    // Initialization of OpeningManager
    _openingmanager   = new OpeningManager();
    // Initialization of EndingManager
    _endingmanager    = new EndingManager();
    
    // Update all delegate
    UpdateDelegator();
    
    /** These init separeted by constructor because need delegator or any reason **/
    _openingmanager   -> Init();
    _mapmanager       -> Init(_map);
    _playermanager    -> Init();
    _charactermanager -> Init(_map);
    _collisionmanager -> Init(_map);
    _eventmanager     -> Init();
    
}

Game::~Game()
{
    free(_controlmanager);
    free(_mapmanager);
    free(_charactermanager);
    free(_playermanager);
    free(_collisionmanager);
    free(_eventmanager);
    free(_statusmanager);
    free(_openingmanager);
    free(_endingmanager);
    free(_menumanager);
}

void Game::Render()
{
    _controlmanager   -> Render();
    _mapmanager       -> Render();
    _charactermanager -> Render();
    _playermanager    -> Render();
    _collisionmanager -> Render();
    _eventmanager     -> Render();
    _statusmanager    -> Render();
    _openingmanager   -> Render();
    _endingmanager    -> Render();
    _menumanager      -> Render();
}

void Game::UpdateDelegator()
{
    
    // Initialization of Delegator ( MapManager::UpdateMuzzleFlashPosition(glm::vec3) )
    Delegator* mapmanager_UpdateMuzzleFlashPosition = Delegate<MapManager>::createDelegator( _mapmanager, &MapManager::UpdateMuzzleFlashPosition );
    // Initialization of Delegator ( MapManager::UpdateLightPosition(glm::vec3) )
    Delegator* mapmanager_UpdateLightPosition = Delegate<MapManager>::createDelegator( _mapmanager, &MapManager::UpdateLightPosition );
    // Initialization of Delegator ( MapManager::Reset() )
    Delegator* mapmanager_Reset = Delegate<MapManager>::createDelegator( _mapmanager, &MapManager::Reset );
    // Initialization of Delegator ( PlayerManager::RecieveCurrentPosition(glm::vec3) )
    Delegator* playermanager_RecieveCurrentPosition = Delegate<PlayerManager>::createDelegator( _playermanager, &PlayerManager::RecieveCurrentPosition );
    // Initialization of Delegator ( PlayerManager::RecieveLastPosition(glm::vec3) )
    Delegator* playermanager_RecieveLastPosition = Delegate<PlayerManager>::createDelegator( _playermanager, &PlayerManager::RecieveLastPosition );
    // Initialization of Delegator ( PlayerManager::UpdateScore(int) )
    Delegator* playermanager_UpdateScore = Delegate<PlayerManager>::createDelegator( _playermanager, &PlayerManager::UpdateScore );
    // Initialization of Delegator ( PlayerManager::RecieveScore() )
    Delegator* playermanager_RecieveScore = Delegate<PlayerManager>::createDelegator( _playermanager, &PlayerManager::RecieveScore );
    // Initialization of Delegator ( PlayerManager::RecieveShoot(int) )
    Delegator* playermanager_RecieveShoot = Delegate<PlayerManager>::createDelegator( _playermanager, &PlayerManager::RecieveShoot );
    // Initialization of Delegator ( PlayerManager::RecieveMagazineAmmo() )
    Delegator* playermanager_RecieveMagazineAmmo = Delegate<PlayerManager>::createDelegator( _playermanager, &PlayerManager::RecieveMagazineAmmo );
    // Initialization of Delegator ( PlayerManager::RecieveExtraAmmo() )
    Delegator* playermanager_RecieveExtraAmmo = Delegate<PlayerManager>::createDelegator( _playermanager, &PlayerManager::RecieveExtraAmmo );
    // Initialization of Delegator ( PlayerManager::RecieveWeaponName(std::string) )
    Delegator* playermanager_RecieveWeaponName = Delegate<PlayerManager>::createDelegator( _playermanager, &PlayerManager::RecieveWeaponName );
    // Initialization of Delegator ( PlayerManager::UpdateLife(int) )
    Delegator* playermanager_UpdateLife = Delegate<PlayerManager>::createDelegator( _playermanager, &PlayerManager::UpdateLife );
    // Initialization of Delegator ( PlayerManager::RecieveLife(int) )
    Delegator* playermanager_RecieveLife = Delegate<PlayerManager>::createDelegator( _playermanager, &PlayerManager::RecieveLife );
    // Initialization of Delegator ( PlayerManager::RecieveMaxLife(int) )
    Delegator* playermanager_RecieveMaxLife = Delegate<PlayerManager>::createDelegator( _playermanager, &PlayerManager::RecieveMaxLife );
    // Initialization of Delegator ( PlayerManager::ProcessActionDrink() )
    Delegator* playermanager_ProcessActionDrink = Delegate<PlayerManager>::createDelegator( _playermanager, &PlayerManager::ProcessActionDrink );
    // Initialization of Delegator ( PlayerManager::ProcessActionMisteryBox() )
    Delegator* playermanager_ProcessActionMisteryBox = Delegate<PlayerManager>::createDelegator( _playermanager, &PlayerManager::ProcessActionMisteryBox );
    // Initialization of Delegator ( PlayerManager::UpdatePlay(int) )
    Delegator* playermanager_UpdatePlay = Delegate<PlayerManager>::createDelegator( _playermanager, &PlayerManager::UpdatePlay );
    // Initialization of Delegator ( PlayerManager::UpdateRender(int) )
    Delegator* playermanager_UpdateRender = Delegate<PlayerManager>::createDelegator( _playermanager, &PlayerManager::UpdateRender );
    // Initialization of Delegator ( PlayerManager::Reset() )
    Delegator* playermanager_Reset = Delegate<PlayerManager>::createDelegator( _playermanager, &PlayerManager::Reset );
    // Initialization of Delegator ( CharacterManager::UpdatePlay(int) )
    Delegator* charactermanager_UpdatePlay = Delegate<CharacterManager>::createDelegator( _charactermanager, &CharacterManager::UpdatePlay );
    // Initialization of Delegator ( CharacterManager::UpdatePlay1(int) )
    Delegator* charactermanager_UpdatePlay1 = Delegate<CharacterManager>::createDelegator( _charactermanager, &CharacterManager::UpdatePlay1 );
    // Initialization of Delegator ( CharacterManager::Reset() )
    Delegator* charactermanager_Reset = Delegate<CharacterManager>::createDelegator( _charactermanager, &CharacterManager::Reset );
    // Initialization of Delegator ( ControlManager::UpdateFOV(float) )
    Delegator* controlmanager_UpdateFOV = Delegate<ControlManager>::createDelegator( _controlmanager, &ControlManager::UpdateFOV );
    // Initialization of Delegator ( ControlManager::UpdateCurrentTerrain(float) )
    Delegator* controlmanager_UpdateCurrentTerrain = Delegate<ControlManager>::createDelegator( _controlmanager, &ControlManager::UpdateCurrentTerrain );
    // Initialization of Delegator ( ControlManager::UpdateLastTerrain(float) )
    Delegator* controlmanager_UpdateLastTerrain = Delegate<ControlManager>::createDelegator( _controlmanager, &ControlManager::UpdateLastTerrain );
    // Initialization of Delegator ( ControlManager::UpdatePosition(glm::vec3) )
    Delegator* controlmanager_UpdatePosition = Delegate<ControlManager>::createDelegator( _controlmanager, &ControlManager::UpdatePosition );
    // Initialization of Delegator ( ControlManager::UpdateDirection(glm::vec3) )
    Delegator* controlmanager_UpdateDirection = Delegate<ControlManager>::createDelegator( _controlmanager, &ControlManager::UpdateDirection );
    // Initialization of Delegator ( ControlManager::RecieveCursorPosition(glm::vec2) )
    Delegator* controlmanager_RecieveCursorPosition = Delegate<ControlManager>::createDelegator( _controlmanager, &ControlManager::RecieveCursorPosition );
    // Initialization of Delegator ( ControlManager::RecieveLastPosition(glm::vec3) )
    Delegator* controlmanager_RecieveLastPosition = Delegate<ControlManager>::createDelegator( _controlmanager, &ControlManager::RecieveLastPosition );
    // Initialization of Delegator ( ControlManager::RecievePosition(glm::vec3) )
    Delegator* controlmanager_RecievePosition = Delegate<ControlManager>::createDelegator( _controlmanager, &ControlManager::RecievePosition );
    // Initialization of Delegator ( ControlManager::RecieveCondition(glm::vec3) )
    Delegator* controlmanager_RecieveCondition = Delegate<ControlManager>::createDelegator( _controlmanager, &ControlManager::RecieveCondition );
    // Initialization of Delegator ( ControlManager::RecieveOrientation(glm::vec3) )
    Delegator* controlmanager_RecieveOrientation = Delegate<ControlManager>::createDelegator( _controlmanager, &ControlManager::RecieveOrientation );
    // Initialization of Delegator ( ControlManager::RecieveValancePosition(glm::vec3) )
    Delegator* controlmanager_RecieveValancePosition = Delegate<ControlManager>::createDelegator( _controlmanager, &ControlManager::RecieveValancePosition );
    // Initialization of Delegator ( ControlManager::RecieveConditionJump(int) )
    Delegator* controlmanager_RecieveConditionJump = Delegate<ControlManager>::createDelegator( _controlmanager, &ControlManager::RecieveConditionJump );
    // Initialization of Delegator ( ControlManager::RecieveConditionFall(int) )
    Delegator* controlmanager_RecieveConditionFall = Delegate<ControlManager>::createDelegator( _controlmanager, &ControlManager::RecieveConditionFall );
    // Initialization of Delegator ( ControlManager::RecieveConditionLand(int) )
    Delegator* controlmanager_RecieveConditionLand = Delegate<ControlManager>::createDelegator( _controlmanager, &ControlManager::RecieveConditionLand );
    // Initialization of Delegator ( ControlManager::RecieveConditionWalk(int) )
    Delegator* controlmanager_RecieveConditionWalk = Delegate<ControlManager>::createDelegator( _controlmanager, &ControlManager::RecieveConditionWalk );
    // Initialization of Delegator ( ControlManager::RecieveConditionRun(int) )
    Delegator* controlmanager_RecieveConditionRun = Delegate<ControlManager>::createDelegator( _controlmanager, &ControlManager::RecieveConditionRun );
    // Initialization of Delegator ( ControlManager::RecieveConditionPose(int) )
    Delegator* controlmanager_RecieveConditionPose = Delegate<ControlManager>::createDelegator( _controlmanager, &ControlManager::RecieveConditionPose );
    // Initialization of Delegator ( ControlManager::RecieveCurrentTerrain(float) )
    Delegator* controlmanager_RecieveCurrentTerrain = Delegate<ControlManager>::createDelegator( _controlmanager, &ControlManager::RecieveCurrentTerrain );
    // Initialization of Delegator ( ControlManager::UpdatePlay(int) )
    Delegator* controlmanager_UpdatePlay = Delegate<ControlManager>::createDelegator( _controlmanager, &ControlManager::UpdatePlay );
    // Initialization of Delegator ( ControlManager::UpdateInvertXAxis(int) )
    Delegator* controlmanager_UpdateInvertXAxis = Delegate<ControlManager>::createDelegator( _controlmanager, &ControlManager::UpdateInvertXAxis );
    // Initialization of Delegator ( ControlManager::UpdateInvertYAxis(int) )
    Delegator* controlmanager_UpdateInvertYAxis = Delegate<ControlManager>::createDelegator( _controlmanager, &ControlManager::UpdateInvertYAxis );
    // Initialization of Delegator ( ControlManager::Reset() )
    Delegator* controlmanager_Reset = Delegate<ControlManager>::createDelegator( _controlmanager, &ControlManager::Reset );
    // Initialization of Delegator ( AudioManager::PlaySound(std::string) )
    Delegator* audiomanager_PlaySound = Delegate<AudioManager>::createDelegator( _audiomanager, &AudioManager::PlaySound );
    // Initialization of Delegator ( AudioManager::PlaySoundOnce(std::string) )
    Delegator* audiomanager_PlaySoundOnce = Delegate<AudioManager>::createDelegator( _audiomanager, &AudioManager::PlaySoundOnce );
    // Initialization of Delegator ( AudioManager::PlaySoundOnceStop(std::string) )
    Delegator* audiomanager_PlaySoundOnceStop = Delegate<AudioManager>::createDelegator( _audiomanager, &AudioManager::PlaySoundOnceStop );
    // Initialization of Delegator ( AudioManager::PlaySoundOnceReset(std::string) )
    Delegator* audiomanager_PlaySoundOnceReset = Delegate<AudioManager>::createDelegator( _audiomanager, &AudioManager::PlaySoundOnceReset );
    // Initialization of Delegator ( AudioManager::LoopSound(std::string) )
    Delegator* audiomanager_LoopSound = Delegate<AudioManager>::createDelegator( _audiomanager, &AudioManager::LoopSound );
    // Initialization of Delegator ( AudioManager::LoopSoundOnce(std::string) )
    Delegator* audiomanager_LoopSoundOnce = Delegate<AudioManager>::createDelegator( _audiomanager, &AudioManager::LoopSoundOnce );
    // Initialization of Delegator ( AudioManager::LoopSoundOnceStop(std::string) )
    Delegator* audiomanager_LoopSoundOnceStop = Delegate<AudioManager>::createDelegator( _audiomanager, &AudioManager::LoopSoundOnceStop );
    // Initialization of Delegator ( AudioManager::LoopSoundOnceReset(std::string) )
    Delegator* audiomanager_LoopSoundOnceReset = Delegate<AudioManager>::createDelegator( _audiomanager, &AudioManager::LoopSoundOnceReset );
    // Initialization of Delegator ( AudioManager::CreateMultiPlaySoundBufferIndex(int) )
    Delegator* audiomanager_CreateMultiPlaySoundBufferIndex = Delegate<AudioManager>::createDelegator( _audiomanager, &AudioManager::CreateMultiPlaySoundBufferIndex );
    // Initialization of Delegator ( AudioManager::MultiPlaySoundOnce(std::string) )
    Delegator* audiomanager_MultiPlaySoundOnce = Delegate<AudioManager>::createDelegator( _audiomanager, &AudioManager::MultiPlaySoundOnce );
    // Initialization of Delegator ( AudioManager::MultiPlaySoundOnceStop(int) )
    Delegator* audiomanager_MultiPlaySoundOnceStop = Delegate<AudioManager>::createDelegator( _audiomanager, &AudioManager::MultiPlaySoundOnceStop );
    // Initialization of Delegator ( AudioManager::MultiPlaySoundOnceReset(int) )
    Delegator* audiomanager_MultiPlaySoundOnceReset = Delegate<AudioManager>::createDelegator( _audiomanager, &AudioManager::MultiPlaySoundOnceReset );
    // Initialization of Delegator ( AudioManager::UpdateMultiAudioindex(int) )
    Delegator* audiomanager_UpdateMultiAudioindex = Delegate<AudioManager>::createDelegator( _audiomanager, &AudioManager::UpdateMultiAudioindex );
    // Initialization of Delegator ( AudioManager::UpdateTargetPosition(glm::vec3) )
    Delegator* audiomanager_UpdateTargetPosition = Delegate<AudioManager>::createDelegator( _audiomanager, &AudioManager::UpdateTargetPosition );
    // Initialization of Delegator ( AudioManager::UpdatePosition(glm::vec3) )
    Delegator* audiomanager_UpdatePosition = Delegate<AudioManager>::createDelegator( _audiomanager, &AudioManager::UpdatePosition );
    // Initialization of Delegator ( AudioManager::PauseSound() )
    Delegator* audiomanager_PauseSound = Delegate<AudioManager>::createDelegator( _audiomanager, &AudioManager::PauseSound );
    // Initialization of Delegator ( AudioManager::ResumeSound() )
    Delegator* audiomanager_ResumeSound = Delegate<AudioManager>::createDelegator( _audiomanager, &AudioManager::ResumeSound );
    // Initialization of Delegator ( AudioManager::Reset() )
    Delegator* audiomanager_Reset = Delegate<AudioManager>::createDelegator( _audiomanager, &AudioManager::Reset );
    // Initialization of Delegator ( OpeningManager::Reset() )
    Delegator* openingmanager_Reset = Delegate<OpeningManager>::createDelegator( _openingmanager, &OpeningManager::Reset );
    // Initialization of Delegator ( CollisionManager::ProcessCreateAABBGroup(int) )
    Delegator* collisionmanager_ProcessCreateAABBGroup = Delegate<CollisionManager>::createDelegator( _collisionmanager, &CollisionManager::ProcessCreateAABBGroup );
    // Initialization of Delegator ( CollisionManager::ProcessCreateOBBGroup(int) )
    Delegator* collisionmanager_ProcessCreateOBBGroup = Delegate<CollisionManager>::createDelegator( _collisionmanager, &CollisionManager::ProcessCreateOBBGroup );
    // Initialization of Delegator ( CollisionManager::ProcessCreateRayOBBGroup(int) )
    Delegator* collisionmanager_ProcessCreateRayOBBGroup = Delegate<CollisionManager>::createDelegator( _collisionmanager, &CollisionManager::ProcessCreateRayOBBGroup );
    // Initialization of Delegator ( CollisionManager::ProcessSubmitAABB(AABB*) )
    Delegator* collisionmanager_ProcessSubmitAABB = Delegate<CollisionManager>::createDelegator( _collisionmanager, &CollisionManager::ProcessSubmitAABB );
    // Initialization of Delegator ( CollisionManager::ProcessSubmitOBB(OBB*) )
    Delegator* collisionmanager_ProcessSubmitOBB = Delegate<CollisionManager>::createDelegator( _collisionmanager, &CollisionManager::ProcessSubmitOBB );
    // Initialization of Delegator ( CollisionManager::ProcessSubmitRayOBB(RayOBB*) )
    Delegator* collisionmanager_ProcessSubmitRayOBB = Delegate<CollisionManager>::createDelegator( _collisionmanager, &CollisionManager::ProcessSubmitRayOBB );
    // Initialization of Delegator ( CollisionManager::UpdateTargetCurrentPosition(glm::vec3) )
    Delegator* collisionmanager_UpdateTargetCurrentPosition = Delegate<CollisionManager>::createDelegator( _collisionmanager, &CollisionManager::UpdateTargetCurrentPosition );
    // Initialization of Delegator ( CollisionManager::UpdateTargetLastPosition(glm::vec3) )
    Delegator* collisionmanager_UpdateTargetLastPosition = Delegate<CollisionManager>::createDelegator( _collisionmanager, &CollisionManager::UpdateTargetLastPosition );
    // Initialization of Delegator ( CollisionManager::UpdateTargetRadius(float) )
    Delegator* collisionmanager_UpdateTargetRadius = Delegate<CollisionManager>::createDelegator( _collisionmanager, &CollisionManager::UpdateTargetRadius );
    // Initialization of Delegator ( CollisionManager::RecieveTerrainCollision(glm::vec3) )
    Delegator* collisionmanager_RecieveTerrainCollision = Delegate<CollisionManager>::createDelegator( _collisionmanager, &CollisionManager::RecieveTerrainCollision );
    // Initialization of Delegator ( CollisionManager::RecieveKabezuriCollision(glm::vec3) )
    Delegator* collisionmanager_RecieveKabezuriCollision = Delegate<CollisionManager>::createDelegator( _collisionmanager, &CollisionManager::RecieveKabezuriCollision );
    // Initialization of Delegator ( FontManager::UpdateText(std::string) )
    Delegator* fontmanager_UpdateText = Delegate<FontManager>::createDelegator( _fontmanager, &FontManager::UpdateText );
    // Initialization of Delegator ( FontManager::UpdateAlign(std::string) )
    Delegator* fontmanager_UpdateAlign = Delegate<FontManager>::createDelegator( _fontmanager, &FontManager::UpdateAlign );
    // Initialization of Delegator ( FontManager::UpdatePosition(glm::vec2) )
    Delegator* fontmanager_UpdatePosition = Delegate<FontManager>::createDelegator( _fontmanager, &FontManager::UpdatePosition );
    // Initialization of Delegator ( FontManager::UpdateColor(glm::vec3) )
    Delegator* fontmanager_UpdateColor = Delegate<FontManager>::createDelegator( _fontmanager, &FontManager::UpdateColor );
    // Initialization of Delegator ( FontManager::UpdateSize(float) )
    Delegator* fontmanager_UpdateSize = Delegate<FontManager>::createDelegator( _fontmanager, &FontManager::UpdateSize );
    // Initialization of Delegator ( FontManager::RenderOpenSansRegular() )
    Delegator* fontmanager_RenderOpenSansRegular = Delegate<FontManager>::createDelegator( _fontmanager, &FontManager::RenderOpenSansRegular );
    // Initialization of Delegator ( FontManager::RenderMarlboro() )
    Delegator* fontmanager_RenderMarlboro = Delegate<FontManager>::createDelegator( _fontmanager, &FontManager::RenderMarlboro );
    // Initialization of Delegator ( FontManager::RenderRockoFLF() )
    Delegator* fontmanager_RenderRockoFLF = Delegate<FontManager>::createDelegator( _fontmanager, &FontManager::RenderRockoFLF );
    // Initialization of Delegator ( FontManager::RenderLater() )
    Delegator* fontmanager_RenderLater = Delegate<FontManager>::createDelegator( _fontmanager, &FontManager::RenderLater );
    // Initialization of Delegator ( FontManager::RenderRomanSerif() )
    Delegator* fontmanager_RenderRomanSerif = Delegate<FontManager>::createDelegator( _fontmanager, &FontManager::RenderRomanSerif );
    // Initialization of Delegator ( EventManager::RecievePerk(int) )
    Delegator* eventmanager_RecievePerk = Delegate<EventManager>::createDelegator( _eventmanager, &EventManager::RecievePerk );
    // Initialization of Delegator ( EventManager::RecieveRound(int) )
    Delegator* eventmanager_RecieveRound = Delegate<EventManager>::createDelegator( _eventmanager, &EventManager::RecieveRound );
    // Initialization of Delegator ( EventManager::RecieveKillCount(int) )
    Delegator* eventmanager_RecieveKillCount = Delegate<EventManager>::createDelegator( _eventmanager, &EventManager::RecieveKillCount );
    // Initialization of Delegator ( EventManager::Reset() )
    Delegator* eventmanager_Reset = Delegate<EventManager>::createDelegator( _eventmanager, &EventManager::Reset );
    // Initialization of Delegator ( EventManager::UpdateKillCount(int) )
    Delegator* eventmanager_UpdateKillCount = Delegate<EventManager>::createDelegator( _eventmanager, &EventManager::UpdateKillCount );
    // Initialization of Delegator ( StatusManager::UpdatePlay(int) )
    Delegator* statusmanager_UpdatePlay = Delegate<StatusManager>::createDelegator( _statusmanager, &StatusManager::UpdatePlay );
    // Initialization of Delegator ( StatusManager::Reset() )
    Delegator* statusmanager_Reset = Delegate<StatusManager>::createDelegator( _statusmanager, &StatusManager::Reset );
    // Initialization of Delegator ( EndingManager::GameOver() )
    Delegator* endingmanager_GameOver = Delegate<EndingManager>::createDelegator( _endingmanager, &EndingManager::GameOver );

    _controlmanager->UpdateDelegator(playermanager_RecieveShoot);
    _mapmanager->UpdateDelegator(collisionmanager_ProcessCreateRayOBBGroup);
    _mapmanager->UpdateDelegator(collisionmanager_ProcessSubmitRayOBB);
    _mapmanager->UpdateDelegator(controlmanager_RecievePosition);
    _mapmanager->UpdateDelegator(controlmanager_RecieveOrientation);
    _mapmanager->UpdateDelegator(playermanager_RecieveShoot);
    _mapmanager->UpdateDelegator(controlmanager_RecieveCurrentTerrain);
    _charactermanager->UpdateDelegator(collisionmanager_ProcessCreateRayOBBGroup);
    _charactermanager->UpdateDelegator(collisionmanager_ProcessSubmitRayOBB);
    _charactermanager->UpdateDelegator(controlmanager_RecievePosition);
    _charactermanager->UpdateDelegator(controlmanager_RecieveOrientation);
    _charactermanager->UpdateDelegator(playermanager_RecieveShoot);
    _charactermanager->UpdateDelegator(audiomanager_PlaySoundOnce);
    _charactermanager->UpdateDelegator(audiomanager_PlaySoundOnceReset);
    _charactermanager->UpdateDelegator(audiomanager_CreateMultiPlaySoundBufferIndex);
    _charactermanager->UpdateDelegator(audiomanager_MultiPlaySoundOnce);
    _charactermanager->UpdateDelegator(audiomanager_MultiPlaySoundOnceStop);
    _charactermanager->UpdateDelegator(audiomanager_MultiPlaySoundOnceReset);
    _charactermanager->UpdateDelegator(audiomanager_UpdateTargetPosition);
    _charactermanager->UpdateDelegator(audiomanager_UpdatePosition);
    _charactermanager->UpdateDelegator(audiomanager_UpdateMultiAudioindex);
    _charactermanager->UpdateDelegator(playermanager_UpdateLife);
    _charactermanager->UpdateDelegator(playermanager_UpdateScore);
    _charactermanager->UpdateDelegator(eventmanager_UpdateKillCount);
    _charactermanager->UpdateDelegator(eventmanager_RecieveRound);
    _charactermanager->UpdateDelegator(playermanager_RecieveLife);
    _charactermanager->UpdateDelegator(collisionmanager_UpdateTargetCurrentPosition);
    _charactermanager->UpdateDelegator(collisionmanager_UpdateTargetLastPosition);
    _charactermanager->UpdateDelegator(collisionmanager_UpdateTargetRadius);
    _charactermanager->UpdateDelegator(collisionmanager_RecieveTerrainCollision);
    _charactermanager->UpdateDelegator(collisionmanager_RecieveKabezuriCollision);
    _playermanager->UpdateDelegator(controlmanager_UpdateFOV);
    _playermanager->UpdateDelegator(audiomanager_PlaySound);
    _playermanager->UpdateDelegator(mapmanager_UpdateMuzzleFlashPosition);
    _playermanager->UpdateDelegator(controlmanager_RecievePosition);
    _playermanager->UpdateDelegator(controlmanager_RecieveLastPosition);
    _playermanager->UpdateDelegator(controlmanager_RecieveOrientation);
    _playermanager->UpdateDelegator(controlmanager_RecieveValancePosition);
    _playermanager->UpdateDelegator(controlmanager_RecieveCondition);
    _eventmanager->UpdateDelegator(collisionmanager_ProcessCreateAABBGroup);
    _eventmanager->UpdateDelegator(collisionmanager_ProcessSubmitAABB);
    _eventmanager->UpdateDelegator(fontmanager_UpdateText);
    _eventmanager->UpdateDelegator(fontmanager_UpdateAlign);
    _eventmanager->UpdateDelegator(fontmanager_UpdatePosition);
    _eventmanager->UpdateDelegator(fontmanager_UpdateColor);
    _eventmanager->UpdateDelegator(fontmanager_UpdateSize);
    _eventmanager->UpdateDelegator(fontmanager_RenderMarlboro);
    _eventmanager->UpdateDelegator(playermanager_UpdateScore);
    _eventmanager->UpdateDelegator(playermanager_RecieveScore);
    _eventmanager->UpdateDelegator(audiomanager_PlaySoundOnce);
    _eventmanager->UpdateDelegator(audiomanager_PlaySoundOnceReset);
    _eventmanager->UpdateDelegator(playermanager_RecieveCurrentPosition);
    _eventmanager->UpdateDelegator(playermanager_ProcessActionDrink);
    _eventmanager->UpdateDelegator(playermanager_ProcessActionMisteryBox);
    _eventmanager->UpdateDelegator(controlmanager_RecieveConditionJump);
    _eventmanager->UpdateDelegator(controlmanager_RecieveConditionFall);
    _eventmanager->UpdateDelegator(controlmanager_RecieveConditionLand);
    _eventmanager->UpdateDelegator(controlmanager_RecieveConditionWalk);
    _eventmanager->UpdateDelegator(controlmanager_RecieveConditionRun);
    _eventmanager->UpdateDelegator(controlmanager_RecieveConditionPose);
    _eventmanager->UpdateDelegator(audiomanager_LoopSoundOnce);
    _eventmanager->UpdateDelegator(audiomanager_LoopSoundOnceStop);
    _eventmanager->UpdateDelegator(audiomanager_LoopSoundOnceReset);
    _eventmanager->UpdateDelegator(endingmanager_GameOver);
    _statusmanager->UpdateDelegator(fontmanager_UpdateText);
    _statusmanager->UpdateDelegator(fontmanager_UpdateAlign);
    _statusmanager->UpdateDelegator(fontmanager_UpdatePosition);
    _statusmanager->UpdateDelegator(fontmanager_UpdateColor);
    _statusmanager->UpdateDelegator(fontmanager_UpdateSize);
    _statusmanager->UpdateDelegator(fontmanager_RenderRomanSerif);
    _statusmanager->UpdateDelegator(playermanager_RecieveScore);
    _statusmanager->UpdateDelegator(playermanager_RecieveMagazineAmmo);
    _statusmanager->UpdateDelegator(playermanager_RecieveExtraAmmo);
    _statusmanager->UpdateDelegator(playermanager_RecieveWeaponName);
    _statusmanager->UpdateDelegator(eventmanager_RecievePerk);
    _statusmanager->UpdateDelegator(playermanager_RecieveLife);
    _statusmanager->UpdateDelegator(playermanager_RecieveMaxLife);
    _statusmanager->UpdateDelegator(eventmanager_RecieveRound);
    _collisionmanager->UpdateDelegator(playermanager_RecieveCurrentPosition);
    _collisionmanager->UpdateDelegator(playermanager_RecieveLastPosition);
    _collisionmanager->UpdateDelegator(controlmanager_UpdatePosition);
    _collisionmanager->UpdateDelegator(controlmanager_RecieveCursorPosition);
    _collisionmanager->UpdateDelegator(controlmanager_RecieveConditionJump);
    _collisionmanager->UpdateDelegator(controlmanager_RecieveConditionFall);
    _collisionmanager->UpdateDelegator(controlmanager_UpdateCurrentTerrain);
    _collisionmanager->UpdateDelegator(controlmanager_UpdateLastTerrain);
    _menumanager->UpdateDelegator(fontmanager_UpdateText);
    _menumanager->UpdateDelegator(fontmanager_UpdateAlign);
    _menumanager->UpdateDelegator(fontmanager_UpdatePosition);
    _menumanager->UpdateDelegator(fontmanager_UpdateColor);
    _menumanager->UpdateDelegator(fontmanager_UpdateSize);
    _menumanager->UpdateDelegator(fontmanager_RenderOpenSansRegular);
    _menumanager->UpdateDelegator(playermanager_UpdatePlay);
    _menumanager->UpdateDelegator(controlmanager_UpdatePlay);
    _menumanager->UpdateDelegator(charactermanager_UpdatePlay);
    _menumanager->UpdateDelegator(audiomanager_ResumeSound);
    _menumanager->UpdateDelegator(audiomanager_PauseSound);
    _menumanager->UpdateDelegator(controlmanager_UpdateInvertXAxis);
    _menumanager->UpdateDelegator(controlmanager_UpdateInvertYAxis);
    _openingmanager->UpdateDelegator(fontmanager_UpdateText);
    _openingmanager->UpdateDelegator(fontmanager_UpdateAlign);
    _openingmanager->UpdateDelegator(fontmanager_UpdatePosition);
    _openingmanager->UpdateDelegator(fontmanager_UpdateColor);
    _openingmanager->UpdateDelegator(fontmanager_UpdateSize);
    _openingmanager->UpdateDelegator(fontmanager_RenderOpenSansRegular);
    _openingmanager->UpdateDelegator(fontmanager_RenderLater);
    _openingmanager->UpdateDelegator(fontmanager_RenderMarlboro);
    _openingmanager->UpdateDelegator(audiomanager_PlaySoundOnce);
    _openingmanager->UpdateDelegator(audiomanager_PlaySoundOnceStop);
    _endingmanager->UpdateDelegator(fontmanager_UpdateText);
    _endingmanager->UpdateDelegator(fontmanager_UpdateAlign);
    _endingmanager->UpdateDelegator(fontmanager_UpdatePosition);
    _endingmanager->UpdateDelegator(fontmanager_UpdateColor);
    _endingmanager->UpdateDelegator(fontmanager_UpdateSize);
    _endingmanager->UpdateDelegator(fontmanager_RenderOpenSansRegular);
    _endingmanager->UpdateDelegator(fontmanager_RenderLater);
    _endingmanager->UpdateDelegator(fontmanager_RenderMarlboro);
    _endingmanager->UpdateDelegator(controlmanager_UpdatePosition);
    _endingmanager->UpdateDelegator(playermanager_RecieveLife);
    _endingmanager->UpdateDelegator(controlmanager_RecievePosition);
    _endingmanager->UpdateDelegator(playermanager_UpdatePlay);
    _endingmanager->UpdateDelegator(controlmanager_UpdatePlay);
    _endingmanager->UpdateDelegator(statusmanager_UpdatePlay);
    _endingmanager->UpdateDelegator(playermanager_UpdateRender);
    _endingmanager->UpdateDelegator(charactermanager_UpdatePlay1);
    _endingmanager->UpdateDelegator(controlmanager_UpdateDirection);
    _endingmanager->UpdateDelegator(mapmanager_Reset);
    _endingmanager->UpdateDelegator(playermanager_Reset);
    _endingmanager->UpdateDelegator(charactermanager_Reset);
    _endingmanager->UpdateDelegator(controlmanager_Reset);
    _endingmanager->UpdateDelegator(eventmanager_Reset);
    _endingmanager->UpdateDelegator(eventmanager_RecieveRound);
    _endingmanager->UpdateDelegator(eventmanager_RecieveKillCount);
    _endingmanager->UpdateDelegator(audiomanager_Reset);
    _endingmanager->UpdateDelegator(audiomanager_PlaySoundOnce);
    _endingmanager->UpdateDelegator(openingmanager_Reset);
    _endingmanager->UpdateDelegator(fontmanager_RenderRomanSerif);
    _endingmanager->UpdateDelegator(statusmanager_Reset);
    _endingmanager->UpdateDelegator(audiomanager_PlaySoundOnceReset);

}


