#include <cmath>
#include "game_rules.hpp"
#pragma once
#include "player_animation.hpp"
#include "flying_stage.hpp"
#include "flying_scenery.hpp"
#include "flying_enemies.hpp"
#include "flying_boss.hpp"
#include "flying_events.hpp"
#include "booster_motion.hpp"
#include "later_monologues.hpp"
#include "later_mechanisms.hpp"
#include "later_fire_traps.hpp"
#include "later_springs.hpp"
#include "later_falling_doll.hpp"
#include "later_special_enemies.hpp"
#include "later_large_guards.hpp"
#include "later_patrol_enemies.hpp"
#include "later_additional_enemies.hpp"
#include "later_yaja_projectiles.hpp"
#include "booster_boss.hpp"
#include "booster_missiles.hpp"
#include "booster_ending.hpp"
#include "jungle_boss_intro.hpp"
#include "final_encounter.hpp"
#include "sixth_stage_scenes.hpp"
#include "player_world_response.hpp"
#include "stage_scene.hpp"
#include "camera_controller.hpp"
#include "pause_inventory.hpp"
#include "projectiles.hpp"
#include "level1_objects.hpp"
#include "level1_enemies.hpp"
#include "enemy_missiles.hpp"
#include "level1_events.hpp"
#include "stage_music.hpp"
#include "stage2_actors.hpp"
#include "stage2_projectiles.hpp"
#include "stage2_extra_projectiles.hpp"
#include "stage2_boss_sequence.hpp"
#include "stage2_player_cinematic.hpp"
#include "stage2_exit.hpp"
#include "stage_fall_sequence.hpp"
#include "later_stage_emitters.hpp"
#include "later_stage_falling_hazards.hpp"
#include "level1_springs.hpp"
#include "companion_hud.hpp"
#include "pickup_hud.hpp"
#include "level1_abilities.hpp"
#include "level1_scene_objects.hpp"
#include "level1_dialogues.hpp"
#include "level1_minigames.hpp"
#include "stage2_minigame.hpp"
#include "extra_minigames.hpp"
#include "../graphics/minigame_canvas.hpp"
#include "stage2_scene_events.hpp"
#include "level1_boss.hpp"
#include "level1_gate_sequence.hpp"
#include <list>
#include <set>
#include <memory>
#include "../graphics/map_renderer.hpp"
#include "../core/input_state.hpp"
#include <map>
#include <functional>
namespace shin {
// Executable integration of recovered player + actual map. Unresolved events are mandatory.
class LevelRuntime:public PlayerContext,public ProjectileContext,public game::ObjectResponseContext {
 friend struct LevelRuntimeTestAccess;
 AssetStore&assets_;game::StageScene scene_;ResourceArchive archive_;std::map<unsigned,int> globals_;
 struct FixedScenery{int x,y;const char*resource;};std::vector<FixedScenery>fixed_scenery_;
 IndexedSurface* target_=nullptr;
 SixthStageScenes sixth_scenes_;FinalEncounter final_encounter_;FinalBossMissiles final_missiles_;std::unique_ptr<GlobalEnding>global_ending_;std::unique_ptr<EndingCredits>ending_credits_;
 std::vector<std::unique_ptr<JungleBoss>>jungle_bosses_;JungleBossIntro jungle_intro_;JungleMissiles jungle_missiles_;
 int jungle_arena_right_=0,jungle_arena_x_=0,jungle_arena_y_=0;
 int view_right_edge(){if(scene_.stage==4&&jungle_arena_right_&&player.at(8)>=jungle_arena_x_-640&&player.at(12)>=jungle_arena_y_-480)return jungle_arena_right_;return int(scene_.map.width)*32;}
 void init_jungle_bounds(){if(scene_.stage!=4)return;int tx=int(scene_.map.width),ty=int(scene_.map.height);for(auto&m:scene_.placements)if(m.type==0x56){tx=std::min(tx,m.cell_x);ty=std::min(ty,m.cell_y);}if(tx==int(scene_.map.width))return;int right=tx;for(int y=std::max(0,ty-15);y<int(scene_.map.height);++y)for(int x=tx;x<int(scene_.map.width);++x){auto&cell=scene_.map.cells[std::size_t(y)*scene_.map.width+x];if(cell.background_tile>=0||cell.foreground_tile>=0)right=std::max(right,x);}jungle_arena_right_=(right+1)*32;jungle_arena_x_=tx*32;jungle_arena_y_=ty*32;}

 std::set<std::size_t> consumed_scene_placements_;std::set<int> consumed_scene_types_;bool boss_intro_consumed_=false,boss_ending_consumed_=false;
 std::vector<bool> spawned_;int movement_enable_ticks_=-1;
 std::unique_ptr<Level1Objects> objects_;std::unique_ptr<Level1Enemies> enemies_;std::unique_ptr<game::FirstBoss> boss_;Level1GateSequence gate_;bool gate_rewarded_=false;EntityState*contact_enemy_=nullptr;unsigned ticks_=0;bool complete_=false;
 std::unique_ptr<PauseInventory> pause_;IndexedSurface paused_frame_;
 std::unique_ptr<EnemyMissiles> enemy_missiles_;
 struct PatrolEntry{std::size_t placement;std::unique_ptr<game::LaterPatrolEnemy>actor;};std::vector<PatrolEntry>patrol_enemies_;game::LaterYajaProjectiles yaja_shots_;
 struct LargeGuardEntry{std::size_t placement;std::unique_ptr<game::LaterLargeGuard>actor;};std::vector<LargeGuardEntry>large_guards_;std::vector<LaterFallingDoll>falling_dolls_;game::LaterAdditionalEnemies additional_enemies_;LaterSprings later_springs_;LaterMechanisms later_mechanisms_;LaterFireTraps fire_traps_;game::LaterSpecialEnemies special_enemies_;std::unique_ptr<BoosterBoss>booster_boss_;BoosterMissiles booster_missiles_;std::unique_ptr<BoosterEnding>booster_ending_;LaterMonologues later_monologues_;BoosterMotion booster_;FlyingEvents flying_events_;FlyingStageMotion flying_;FlyingScenery flying_scenery_;game::FlyingEnemyManager flying_enemies_;std::unique_ptr<FlyingBoss> flying_boss_;std::vector<game::EntityPayload> flying_missiles_;
 StageMusic music_{0};
 std::unique_ptr<game::Stage2Actors> later_actors_;
 std::unique_ptr<game::Stage2Projectiles> later_projectiles_;
 std::unique_ptr<game::Stage2ExtraProjectiles> flame_projectiles_;
 game::Stage2BossSequence later_sequence_;Stage2PlayerCinematic later_player_;
 std::unique_ptr<Stage2ExitCutscene> later_exit_;
 void update_later_sequence(bool action,bool advance){
  if(!later_actors_)return;
  for(int kind:later_actors_->spawn_requests){
   if(kind==0x31){if(boss_intro_consumed_)continue;boss_intro_consumed_=true;auto*b=later_actors_->boss();if(!b)throw std::runtime_error("missing stage2 boss");// Bind the arena to original map direction markers, independent of widescreen visibility.
    int left=-1,right=-1,bx=b->payload.at(4)+76,by=b->payload.at(8)+189;
    for(const auto&o:objects_->objects())if(o.payload.kind==0x3f&&std::abs(o.payload.words[2]-by)<96){int x=o.payload.words[1],direction=o.payload.words[16]&255;if(direction==2&&x<bx)left=std::max(left,x);if(direction==1&&x>bx&&(right<0||x<right))right=x+15;}
    if(left>=0&&right>left)b->set_patrol_bounds(left,right);
    later_sequence_.begin_intro(*b,now_ms(),player.at(0x5c),true);}
   else if(kind==0x2d){if(boss_ending_consumed_)continue;boss_ending_consumed_=true;later_sequence_.begin_ending(now_ms(),player.at(0x5c),true);}
   else throw std::runtime_error("unknown stage2 sequence");
  }later_actors_->spawn_requests.clear();
  if(advance)later_sequence_.tick(now_ms(),action,player.at(8));
  for(auto helper:later_sequence_.player_calls)if(!later_player_.helpercall(helper,player))throw std::runtime_error("unknown stage2 player helper");later_sequence_.player_calls.clear();
  if(later_sequence_.restore_state_requested){player.at(0x5c)=later_sequence_.restore_player_state;later_sequence_.restore_state_requested=false;}
  for(auto&file:later_sequence_.sounds)if(play_sound_file)play_sound_file(file);later_sequence_.sounds.clear();
  for(auto request:later_player_.spawn_requests){if(request!=0x43e610)throw std::runtime_error("unknown stage2 exit");later_exit_=std::make_unique<Stage2ExitCutscene>();}later_player_.spawn_requests.clear();
 }

 void ensure_later_actors(){if(!later_actors_)later_actors_=std::make_unique<game::Stage2Actors>(assets_,scene_.map,scene_.collision_blocks,player);}
 std::unique_ptr<Level1Springs> springs_;
 CompanionHud companion_;
 std::unique_ptr<Level1Abilities> abilities_;
 Level1SceneObjects scene_objects_;Level1Dialogues dialogues_;
 StageFallSequence fall_;LaterStageEmitters emitters_;LaterStageFallingHazards falling_hazards_;
 ExtraMinigames extra_game_;Stage2Minigame mole_game_;Stage2SceneEvents scene_events_;
 Level1Minigames minigames_;unsigned minigame_ticks_=0;
 void ensure_scene_callbacks(){
  emitters_.spawn=[&](unsigned,unsigned,unsigned,int,int x,int y){falling_hazards_.spawn(x,y);};
  emitters_.respawn=[&](int,int x,int y){const auto&im=assets_.image("hw6.app");for(std::size_t i=0;i<scene_.placements.size();++i){auto&m=scene_.placements[i];if(m.type==0x23&&m.pixel_x(placement_sprite(m).width)==x&&m.pixel_y(placement_sprite(m).height)==y)spawned_[i]=false;}};
  later_monologues_.spawn_actor=[&](int x,int y){special_enemies_.spawn_cinematic(x,y);};
  later_monologues_.resume_actor=[&](){special_enemies_.resume_cinematic();};
  scene_objects_.respawn=[&](int index){if(index>=0&&std::size_t(index)<spawned_.size())spawned_[index]=false;};
  dialogues_.resume_enemy=[&](const Level1Dialogues::Actor&a){enemies_->spawn_stationary(a.x,a.y);};
 }
 template<class Visit> void visit_later_weapon_targets(Visit visit){
  for(auto*e:additional_enemies_.targets())visit(*e,game::LaterAdditionalEnemies::rectangle(*e),[&,e](){return additional_enemies_.hurt(*e);});
  for(auto*e:special_enemies_.targets())if(e->at(0x50)!=5)visit(*e,game::LaterSpecialEnemies::rectangle(*e),[&,e](){return special_enemies_.hurt(*e);});
  for(auto&entry:patrol_enemies_){auto*a=entry.actor.get();auto&e=a->payload;if(e.state!=2&&e.at(0x58)!=(e.kind==0x7b?3:4))visit(e,game::LaterPatrolEnemy::rectangle(e),[a](){a->hurt();return true;});}
  for(auto&entry:large_guards_){auto*a=entry.actor.get();auto&e=a->payload;if(e.state!=2&&e.at(0x58)!=4)visit(e,game::LaterLargeGuard::rectangle(e),[a](){a->hurt();return true;});}
  for(auto&entry:jungle_bosses_){auto*b=entry.get();if(!b->defeated&&!b->dying()&&!b->cinematic)visit(b->payload,b->rectangle(),[b](){return b->hurt();});}
  if(final_encounter_.completed())for(auto*b:{final_encounter_.robot.get(),final_encounter_.nurse.get()})if(b&&!b->defeated&&!b->dying())visit(b->payload,b->rectangle(),[b](){return b->hurt();});
  if(booster_boss_&&booster_boss_->payload.state!=2)visit(booster_boss_->payload,BoosterBoss::rectangle(),[&](){return booster_boss_->hurt();});
 }
 void ensure_abilities(){if(abilities_)return;abilities_=std::make_unique<Level1Abilities>(assets_);
  abilities_->nearest_target=[&](int x,int,int&tx,int&ty){bool found=false;int best=10000;for(auto*enemy:enemies_->targets()){int distance=std::abs(enemy->x-x);if(distance<best){found=true;best=distance;tx=enemy->x;ty=enemy->y;}}
   if(later_actors_)for(auto*e:later_actors_->targets()){int distance=std::abs(e->at(4)-x);if(distance<best){found=true;best=distance;tx=e->at(4);ty=e->at(8);}}
   if(boss_&&boss_->payload.state==0){auto&e=boss_->payload;int distance=std::abs(e.at(4)-x);if(distance<best){found=true;best=distance;tx=e.at(4);ty=e.at(8);}}visit_later_weapon_targets([&](game::EntityPayload&e,const game::CollisionRect&,auto){int distance=std::abs(e.at(4)-x);if(e.kind!=0x7e&&distance<best){best=distance;found=true;tx=e.at(4);ty=e.at(8);}});return found;};
  abilities_->clear_enemies=[&]{for(auto*enemy:enemies_->targets())enemies_->defeat(*enemy);if(later_actors_)for(auto*e:later_actors_->targets())later_actors_->hurt(*e);if(boss_&&boss_->payload.state==0&&boss_->payload.at(0x74)>0)sound(boss_->hurt());visit_later_weapon_targets([](game::EntityPayload&,const game::CollisionRect&,auto hurt){hurt();});};
  abilities_->damage=[&](const game::CollisionRect&r,int x,int y){int count=0;
   if(later_actors_)for(auto*e:later_actors_->targets())if(!game::Stage2Actors::dying(*e)&&game::intersects(r,x,y,game::Stage2Actors::rectangle(*e),e->at(4),e->at(8))&&later_actors_->hurt(*e))++count;
   for(auto*enemy:enemies_->targets())if(enemy->state!=(enemy->map_type==0?3:4)&&game::intersects(r,x,y,{28,14,46,71},enemy->x,enemy->y)){enemies_->defeat(*enemy);++count;}
   if(boss_&&boss_->payload.state==0&&boss_->payload.at(0x50)!=4){auto&e=boss_->payload;if(game::intersects(r,x,y,{e.at(0x20),e.at(0x24),e.at(0x28),e.at(0x2c)},e.at(4),e.at(8))){sound(boss_->hurt());++count;}}visit_later_weapon_targets([&](game::EntityPayload&e,const game::CollisionRect&target_rect,auto hurt){if(game::intersects(r,x,y,target_rect,e.at(4),e.at(8))&&hurt())++count;});return count;
  };
 }
 double scene_view_left_=0,scene_view_top_=0;bool scene_panning_=false;unsigned scene_pan_time_=0;int scene_pan_width_=0;
 void pan_scene_view(IndexedSurface&target){
  int left=player.at(8),right=left+player.at(0x10),top=player.at(12)-9,bottom=player.at(12)+player.at(0x14);
  bool focus=cinematic_active(),vertical_focus=focus;
  auto include=[&](int x,int y,int w,int h){left=std::min(left,x);right=std::max(right,x+w);top=std::min(top,y);bottom=std::max(bottom,y+h);};
  if(dialogues_.active())for(auto&a:dialogues_.actors)if(!a.removed){
   const char*const*names=a.identity==0?dialogue_data::n0:a.identity==1?dialogue_data::n1:a.identity==2?dialogue_data::n2:dialogue_data::n3;
   for(int i=0;i<(a.identity<2?4:a.identity==2?3:2);++i){const auto&im=assets_.image(names[i]);include(a.x,a.y,im.width,im.height);}
  }
  if(later_sequence_.active()&&later_actors_)if(auto*b=later_actors_->boss();b&&b->cinematic){
   auto&e=b->payload;const auto&im=assets_.image("2#421.app");include(e.at(4),e.at(8),im.width,im.height);
  }
  if(jungle_intro_.active())for(auto&b:jungle_bosses_)if(b&&!b->defeated){auto&e=b->payload;include(e.at(4),e.at(8),e.at(0x64),e.at(0x68));}
  if(sixth_scenes_.active())sixth_scenes_.visit_actors([&](int type,int x,int y){
   const char*name=type==0x3b?"mrh1aa.raw":type==0x29?"ana2aa.raw":type==0x27?"7#731.raw":"7#741.raw";
   const auto&im=assets_.image(name);include(x,y,im.width,im.height);
  });
  if(later_monologues_.active()&&later_monologues_.type()==0x72)for(auto*e:special_enemies_.targets())if(e->at(0x50)==5){
   // The ghost enters from above; frame its landing area without following its birth Y.
   include(e->at(4),std::max(e->at(8),bottom-200),std::max(100,e->at(0x64)),200);
  }
  if(gate_.active()){const auto&im=assets_.image("recall.app");include(player.at(8),player.at(12)-182,im.width,im.height);}
  if(final_encounter_.active()){int l=0,r=0,t=0,b=0;if(final_encounter_.focus_bounds(l,r,t,b))include(l,t,r-l,b-t);else include(player.at(8)+460,player.at(12),100,player.at(0x14));}
  // Pan only as far as needed to contain the cast, preserving the normal composition.
  auto fit=[](int normal,int lo,int hi,int extent,int world){
   constexpr int margin=16;int low=hi+margin-extent,high=lo-margin;
   int desired=low<=high?std::clamp(normal,low,high):(lo+hi-extent)/2;
   return std::clamp(desired,0,std::max(0,world-extent));
  };
  int normal=camera_x()-target.origin_x,desired=focus?fit(player.at(8)+player.at(0x10)/2-target.width/2,left,right,target.width,view_right_edge()):normal;unsigned now=now_ms();
  if(scene_pan_width_!=target.width){scene_panning_=false;scene_pan_width_=target.width;}
  if(focus&&!scene_panning_){scene_view_left_=normal;scene_view_top_=camera_y()-target.origin_y;scene_pan_time_=now;scene_panning_=true;}
  if(!scene_panning_)return;
  unsigned dt=std::min(100u,now-scene_pan_time_);scene_pan_time_=now;double delta=desired-scene_view_left_;double step=delta*(1-std::exp(-double(dt)/220.0));double limit=double(dt)*0.32;scene_view_left_+=std::clamp(step,-limit,limit);
  int desired_top=vertical_focus?fit(camera_y()-target.origin_y,top,bottom,target.height,int(scene_.map.height)*32):camera_y()-target.origin_y;double dy=desired_top-scene_view_top_;scene_view_top_+=std::clamp(dy*(1-std::exp(-double(dt)/220.0)),-limit,limit);target.origin_y=camera_y()-int(std::lround(scene_view_top_));
  if(!focus&&std::abs(desired-scene_view_left_)<0.5&&std::abs(desired_top-scene_view_top_)<0.5){scene_panning_=false;scene_view_left_=desired;}
  target.origin_x=camera_x()-int(std::lround(scene_view_left_));
 }
 // Independent 640x480 story panels must not inherit the preceding world-camera origin.
 void draw_story_panel(IndexedSurface&target,const std::vector<MenuDraw>&draws){
  int ox=target.origin_x,oy=target.origin_y;bool clip=target.clip_core;
  target.origin_x=(target.width-640)/2;target.origin_y=(target.height-480)/2;target.clip_core=false;target.border_rgb.clear();target.clear();
  for(const auto&d:draws)target.blit(assets_.image(d.resource),d.x,d.y,d.color_key);
  target.origin_x=ox;target.origin_y=oy;target.clip_core=clip;
 }
 const IndexedSurface& placement_sprite(const game::MapPlacement&m){return assets_.image(map_placement_asset(m.type));}
 void prepare_static_scene_objects(){fixed_scenery_.clear();if(scene_.stage==2)flying_scenery_.prepare(scene_.placements,assets_);for(auto&m:scene_.placements)if(m.type==0x74){auto&im=placement_sprite(m);fixed_scenery_.push_back({m.pixel_x(im.width),m.pixel_y(im.height),map_placement_asset(m.type)});}for(std::size_t i=0;i<scene_.placements.size();++i){auto&m=scene_.placements[i];if(m.type!=0x5f)continue;const auto&im=assets_.image("fire1.raw");fire_traps_.spawn(int(i),m.pixel_x(placement_sprite(m).width),m.pixel_y(placement_sprite(m).height));auto&e=fire_traps_.objects.back().e;e.at(0xc)=im.width;e.at(0x10)=im.height;spawned_[i]=true;}for(std::size_t i=0;i<scene_.placements.size();++i){auto&m=scene_.placements[i];if(!Level1SceneObjects::supports(m.type))continue;int w=32,h=32;const char*resource=m.type==0x57?"mole_m.app":m.type==0x7b?"gamecar.app":m.type==0x7c?"gamech_b.app":m.type==0x7d?"gamepuzl.app":m.type==0x7e?"gamecard.app":nullptr;if(resource){const auto&im=assets_.image(resource);w=im.width;h=im.height;}scene_objects_.spawn(m.type,m.pixel_x(w),m.pixel_y(h),int(i));spawned_[i]=true;}}
 void prepare_all_platforms(){for(std::size_t i=0;i<scene_.placements.size();++i){auto&m=scene_.placements[i];if(LaterMechanisms::supports(m.type)){auto&im=placement_sprite(m);later_mechanisms_.spawn(m.type,m.pixel_x(im.width),m.pixel_y(im.height),int(i));spawned_[i]=true;}
   else if(m.type==0x23){auto&im=placement_sprite(m);emitters_.add(m.pixel_x(im.width),m.pixel_y(im.height));spawned_[i]=true;}
  }}
 void prepare_all_springs(){
  springs_=std::make_unique<Level1Springs>(assets_,scene_.placements);
  for(std::size_t i=0;i<scene_.placements.size();++i){auto&m=scene_.placements[i];if(!LaterSprings::supports(m.type))continue;auto&im=assets_.image(LaterSprings::resource(m.type));later_springs_.spawn(m.type,m.pixel_x(placement_sprite(m).width),m.pixel_y(placement_sprite(m).height),int(i));spawned_[i]=true;}
 }
 void rearm_scenery(std::size_t i,int type,int x,int y,int w,int h){if(i>=scene_.placements.size())return;auto&m=scene_.placements[i];m.type=type;int ax=x+w/2-16,ay=y+h-32;m.cell_x=int(std::floor(double(ax)/32));m.cell_y=int(std::floor(double(ay)/32));m.offset_x=ax-m.cell_x*32;m.offset_y=ay-m.cell_y*32;spawned_[i]=false;objects_->rearm(i,m);}
 void relocate_enemy(std::size_t index,int x,int y){
  if(index>=scene_.placements.size())return;auto&m=scene_.placements[index];
  const char*name=m.type==0?"c1_f.app":m.type==1?"c2_f.app":m.type==2?"c3_f.app":m.type==31?"cht11cd.app":m.type==36?"cht122dd.app":m.type==0x25?"cht121bd.app":m.type==0x1d?"chch1bd.app":m.type==0x1e?"chch2cc.app":"cht144ac.app";
  auto&im=assets_.image(name);int ax=x+im.width/2,ay=y+im.height;
  m.cell_x=int(std::floor(double(ax)/32));m.cell_y=int(std::floor(double(ay)/32));m.offset_x=ax-m.cell_x*32-16;m.offset_y=ay-(m.cell_y+1)*32;for(std::size_t i=0;i<scene_.placements.size();++i)if(i!=index&&!spawned_[i]&&scene_.placements[i].cell_x==m.cell_x&&scene_.placements[i].cell_y==m.cell_y){spawned_[index]=true;return;}spawned_[index]=false;
 }
 bool reload_pending_=false,return_to_menu_=false,game_over_=false,game_over_space_=false;
 unsigned game_over_started_=0;
 struct Explosion{int x,y,age=0;bool fresh=true;};std::vector<Explosion> explosions_;
 void queue_water_explosion(Projectile&p){if(p.kind==ProjectileKind::water_bomb&&!p.explosion_queued){p.explosion_queued=true;explosions_.push_back({p.at(4)-37,p.at(8)-50});}}
 void award_hit(){if(score_awards_life(++player.at(0x70))){++player.at(0x8c);sound(0x475200);}}
 void reset_world(){scene_panning_=false;sixth_scenes_=SixthStageScenes{};final_encounter_=FinalEncounter{};final_missiles_=FinalBossMissiles{};global_ending_.reset();ending_credits_.reset();falling_dolls_.clear();jungle_bosses_.clear();jungle_intro_=JungleBossIntro{};jungle_missiles_=JungleMissiles{};additional_enemies_=game::LaterAdditionalEnemies{};later_springs_=LaterSprings{};patrol_enemies_.clear();yaja_shots_=game::LaterYajaProjectiles{};booster_boss_.reset();booster_missiles_=BoosterMissiles{};booster_ending_.reset();large_guards_.clear();later_mechanisms_=LaterMechanisms{};fire_traps_=LaterFireTraps{};special_enemies_=game::LaterSpecialEnemies{};later_monologues_=LaterMonologues{};booster_=BoosterMotion{};flying_events_=FlyingEvents{};flying_=FlyingStageMotion{};flying_scenery_=FlyingScenery{};flying_enemies_.clear();flying_boss_.reset();flying_missiles_.clear();globals_[0x461754]=0;consumed_scene_placements_.clear();consumed_scene_types_.clear();boss_intro_consumed_=boss_ending_consumed_=false;scene_.load(archive_,scene_.stage);spawned_.assign(scene_.placements.size(),false);movement_enable_ticks_=-1;shots_.clear();explosions_.clear();object_shots_.clear();world_shots_.clear();enemy_missiles_.reset();springs_.reset();boss_.reset();gate_=Level1GateSequence{};gate_rewarded_=false;companion_=CompanionHud{};globals_[0x46cda8]=0;
  extra_game_=ExtraMinigames{};mole_game_=Stage2Minigame{};scene_events_=Stage2SceneEvents{};fall_=StageFallSequence{};emitters_=LaterStageEmitters{};falling_hazards_=LaterStageFallingHazards{};later_actors_.reset();later_projectiles_.reset();flame_projectiles_.reset();later_sequence_=game::Stage2BossSequence{};later_player_=Stage2PlayerCinematic{};later_exit_.reset();abilities_.reset();scene_objects_=Level1SceneObjects{};dialogues_=Level1Dialogues{};objects_=std::make_unique<Level1Objects>(assets_,scene_.placements);enemies_=std::make_unique<Level1Enemies>(assets_,scene_.map,scene_.collision_blocks,player);prepare_all_springs();prepare_all_platforms();prepare_static_scene_objects();ensure_scene_callbacks();
  objects_->gate_contact=[&](PlayerPayload&p){gate_.start(p,now_ms());};reload_pending_=false;
 }
 std::list<std::unique_ptr<Projectile>> shots_;std::map<int,Projectile*> object_shots_,world_shots_;int next_handle_=1;

public:
 PlayerPayload player;
 std::function<void(const std::string&)> play_sound_file,play_sound_loop,stop_sound_file;
 std::function<int(unsigned)> original_sound;
 std::function<void(unsigned)> play_music;
 std::function<unsigned()> clock_ms;
 std::function<void(int)> unlock_bonus;
 unsigned now_ms()const{return clock_ms?clock_ms():ticks_*16;}
 std::function<void()> stop_music;
 std::function<void(const char*)> change_palette;
 int sound(unsigned id)override{if(original_sound)return original_sound(id);throw std::runtime_error("original sound not bound");}
 bool subgame_active()const{return minigames_.active()||mole_game_.active()||extra_game_.active();}
 bool cinematic_active()const{return dialogues_.active()||gate_.active()||later_monologues_.active()||scene_events_.active()||later_player_.active()||later_sequence_.active()||sixth_scenes_.active()||final_encounter_.active()||jungle_intro_.active()||flying_events_.active();}
 bool scene_event_active()const{return scene_events_.active();}
 bool completed()const{return complete_;}
 int stage_index()const{return scene_.stage;}
 unsigned map_width()const{return scene_.map.width;}
 bool wants_menu()const{return return_to_menu_;}
 bool game_over()const{return game_over_;}
 std::function<int(unsigned,PlayerPayload&,std::initializer_list<std::int64_t>)> event;
 LevelRuntime(AssetStore&a,const std::string&path,int stage):assets_(a),archive_(path),music_(unsigned(stage)){scene_.load(archive_,stage);init_jungle_bounds();spawned_.resize(scene_.placements.size());initialize_player_payload(player,0,0);globals_[0x45c0a8]=2;globals_[0x46c500]=2;globals_[0x461428]=stage;globals_[0x46142c]=-1;objects_=std::make_unique<Level1Objects>(assets_,scene_.placements);enemies_=std::make_unique<Level1Enemies>(assets_,scene_.map,scene_.collision_blocks,player);objects_->gate_contact=[&](PlayerPayload&p){gate_.start(p,now_ms());};prepare_all_springs();prepare_all_platforms();prepare_static_scene_objects();}
 void debug_boss_entry(int weapon_count=10){
  static constexpr int triggers[]={0x12,0x20,0x21,0x7a,0x56,0x70,0x74};int type=triggers[scene_.stage];
  for(auto&m:scene_.placements)if(m.type==type){initialize_player_payload(player,0,0);player.at(0x100)=1;for(unsigned o=0xc8;o<=0xe4;o+=4)player.at(o)=weapon_count;
   if(scene_.stage>=4){player.at(8)=scene_.stage==6?m.pixel_x(assets_.image("hang.raw").width)-40:m.pixel_x(32);player.at(12)=m.pixel_y(32)-player.at(0x38)+16;}
   else{int anchor_x=m.cell_x*32+16+m.offset_x,anchor_y=(m.cell_y+1)*32+m.offset_y;player.at(8)=std::max(0,anchor_x-300);player.at(12)=anchor_y-player.at(0x14);if(scene_.stage==2){const auto&im=assets_.image("chh125bc.raw");player.at(8)=std::max(0,m.pixel_x(placement_sprite(m).width)-220);player.at(12)=std::max(0,m.pixel_y(placement_sprite(m).height)+60);}}
   player.at(0x4c)=player.at(8);player.at(0x50)=player.at(12);auto&c=scene_.camera;int x=std::clamp(player.at(8)-280,0,std::max(0,int(scene_.map.width)*32-640)),y=std::clamp(player.at(12)-260,0,std::max(0,int(scene_.map.height)*32-480));c.pixel_x=x;c.tile_x=x/32;c.offset_x=x%32;c.pixel_y=y;c.tile_y=y/32;c.offset_y=y%32;scene_panning_=false;return;}
  throw std::runtime_error("stage boss/ending trigger missing");
 }
 void debug_final_boss_entry(bool armed=false){if(scene_.stage!=6)throw std::runtime_error("final boss entry requires stage seven");debug_boss_entry(armed?10:0);}
 bool paused()const{return bool(pause_);}std::size_t projectile_count()const{return shots_.size();}
 std::size_t enemy_projectile_count()const{return enemy_missiles_?enemy_missiles_->missiles.size():0;}
 std::size_t explosion_count()const{return explosions_.size();}
 bool present()const override{return true;}
 int register_collision(unsigned registry,unsigned,Projectile&p)override{int id=next_handle_++;(registry==0x434b40?object_shots_:world_shots_)[id]=&p;return id;}
 void unregister_collision(unsigned registry,int id)override{auto& table=registry==0x434bb0?object_shots_:world_shots_;if(!table.erase(id))throw std::runtime_error("missing projectile registration");}
 void release_allocation(int token)override{if(token)throw std::runtime_error("unexpected original allocation token");}
 void render(Projectile&p)override{if(target_)target_->blit(assets_.image(p.resource()),p.at(0x2c),p.at(0x30));}
 void entity_collision(unsigned,Projectile&,unsigned,void*)override{throw std::runtime_error("projectile object response not bound");}
 int call(unsigned id,Projectile&p,std::initializer_list<std::int64_t>args)override{
  auto v=std::vector<std::int64_t>(args);if(id==0x431130){game::CollisionWorld world(scene_.map,scene_.collision_blocks);return world.body({p.at(0x14),p.at(0x18),p.at(0x1c),p.at(0x20)},int(v.at(1)),int(v.at(2)));}
  if(id==0x42b300){queue_water_explosion(p);return 0;}
  throw std::runtime_error("projectile event not bound: "+std::to_string(id));
 }
 void spawn_shot(ProjectileKind kind,int x,int y,int vx){auto p=std::make_unique<Projectile>();const auto&image=assets_.image(kind==ProjectileKind::ball?"ball.app":"wbomb.app");initialize_projectile(*p,kind,x,y,vx,image.width,image.height,*this);shots_.push_back(std::move(p));}
 int call(unsigned id,PlayerPayload&p,game::EntityPayload&e,std::initializer_list<std::int64_t>args)override{
  if(id==0x443610||id==0x42ad00){for(auto*b:{final_encounter_.robot.get(),final_encounter_.nurse.get()})if(b&&&b->payload==&e){b->hurt();return 0;}}
  if(id==0x405360||id==0x444ad0){for(auto&b:jungle_bosses_)if(&b->payload==&e){b->hurt();return 0;}}
  if(later_actors_&&later_actors_->call_helper(id,e))return 0;
  if(id==0x4286e0||id==0x403720){for(auto&entry:large_guards_)if(&entry.actor->payload==&e){entry.actor->hurt();return 0;}}
  if((id==0x43b5e0||id==0x420ea0)&&additional_enemies_.hurt(e))return 0;
  if(id==0x432f90&&special_enemies_.hurt(e))return 0;
  if(id==0x42c4c0)for(auto&o:fire_traps_.objects)if(&o.e==&e){e.state=2;return 0;}
  if(id==0x41dab0||id==0x43cd20||id==0x42c4c0||id==0x41e230){for(auto&entry:patrol_enemies_)if(&entry.actor->payload==&e){entry.actor->hurt();return 0;}}
  if(unlock_bonus){int bonus=id==0x401d10?0:id==0x41c810?1:id==0x41ba30?2:id==0x43d120?3:id==0x4245a0?4:-1;if(bonus>=0)unlock_bonus(bonus);}
  if(id==0x41c810||id==0x4245a0){extra_game_.start(id==0x41c810?1:4);return 0;}
  if(id==0x43d120){mole_game_.start();return 0;}
  if(id==0x401d10||id==0x41ba30){minigames_.start(id==0x401d10?0x7b:0x7c);minigame_ticks_=0;return 0;}
  if(id==0x434f90){e.at(0x50)=1;e.at(0x20)=e.at(0x24)=0;e.at(0x1c)=0x46dae0;e.at(0x28)=5;for(auto&o:later_springs_.objects)if(&o.e==&e){e.at(0x28)=3;break;}e.at(0x2c)=10;auto&flag=globals_[0x46daf4];unsigned voice=flag?0x77dfb8:0x77dfc4;flag=!flag;return sound(voice);}
  if(id==0x44de50||id==0x4242a0||id==0x420660||id==0x440a10||id==0x43aa70||id==0x428b30||id==0x41f7f0||id==0x4012e0||id==0x441290){
   struct PickupVoice{unsigned helper,flag,first,second;};static constexpr PickupVoice voices[]={{0x44de50,0x471bb4,0x7903b4,0x7903b0},{0x4242a0,0x461244,0x478b08,0x478b00},{0x420660,0x460cfc,0x478500,0x478504},{0x440a10,0x46f90c,0x78da70,0x78da6c},{0x43aa70,0x46e6b4,0x77ebb0,0x77ebb4},{0x428b30,0x461e0c,0x77c2c0,0x77c2c4},{0x41f7f0,0x460bfc,0x4783a8,0x4783b0},{0x4012e0,0x45a06c,0x473070,0x473074},{0x441290,0x46fcbc,0x78e054,0x78e058}};
   for(auto&v:voices)if(v.helper==id){e.at(0x3c)=1;e.at(0x24)=companion_.screen_pixel_x()+40;e.at(0x28)=companion_.screen_pixel_y()+40;e.at(0x38)=-15;unsigned voice=globals_[v.flag]?v.second:v.first;globals_[v.flag]=!globals_[v.flag];return sound(voice);}
  }
  if(id==0x447f70||id==0x43e450){e.at(0x3c)=1;e.at(0x24)=companion_.screen_pixel_x()+40;e.at(0x28)=companion_.screen_pixel_y()+40;e.at(0x38)=-15;auto&flag=globals_[id==0x447f70?0x470d5c:0x46f2a4];unsigned voice=id==0x447f70?(flag?0x78f690:0x78f68c):(flag?0x77fb84:0x77fb78);flag=!flag;return sound(voice);}
  if(id==0x43c0a0||id==0x414820||id==0x42ebb0){e.at(0x3c)=1;e.at(0x24)=companion_.screen_pixel_x()+40;e.at(0x28)=companion_.screen_pixel_y()+40;e.at(0x38)=-15;unsigned flag=id==0x43c0a0?0x46e8fc:id==0x414820?0x45eaac:0x46cc94;unsigned voice=id==0x43c0a0?(globals_[flag]?0x77ef58:0x77ef54):id==0x414820?(globals_[flag]?0x4767d8:0x4767e0):(globals_[flag]?0x77d194:0x77d1a0);globals_[flag]=!globals_[flag];return sound(voice);}
  if((id==0x403e50||id==0x4463b0||id==0x4393d0)&&contact_enemy_){enemies_->defeat(*contact_enemy_);return 0;}
  if(id==0x422680&&boss_)return sound(boss_->hurt());
  if(id==0x434ad0){auto v=std::vector<std::int64_t>(args);
   auto rect=[&](int address)->game::CollisionRect{if(address==0x476288)return {p.at(0x2c),p.at(0x38)-6,p.at(0x34),p.at(0x38)};
    if(address>=0x20000&&address+12<0x20200){unsigned o=unsigned(address-0x20000);return {e.at(o),e.at(o+4),e.at(o+8),e.at(o+12)};}
    throw std::runtime_error("unknown rectangle reference");};
   return game::intersects(rect(int(v.at(0))),int(v.at(1)),int(v.at(2)),rect(int(v.at(3))),int(v.at(4)),int(v.at(5)))?1:0;}

  if(id==0x40d950){p.at(0x5c)=12;p.at(0x18)=0x45c778;p.at(0x1c)=p.at(0x20)=0;p.at(0x24)=1;p.at(0x28)=100;return 0;}
  throw std::runtime_error("object event not bound: "+std::to_string(id));
 }
 int&global(unsigned a)override{return globals_[a];}
 int camera_x()const override{return scene_.camera.pixel_x;}int camera_y()const override{return scene_.camera.pixel_y;}
 void render(PlayerPayload&p)override{if(target_)render_player(p,assets_,*target_);}
 int call(unsigned id,PlayerPayload&p,std::initializer_list<std::int64_t> args)override{
  auto v=std::vector<std::int64_t>(args);game::CollisionWorld world(scene_.map,scene_.collision_blocks);game::CollisionRect r={p.at(0x2c),p.at(0x30),p.at(0x34),p.at(0x38)};
  if(id==0x431d60)return world.ground(r,int(v.at(1)),int(v.at(2)));
  if(id==0x431f90)return world.left(r,int(v.at(1)),int(v.at(2)));
  if(id==0x4320c0)return world.right(r,int(v.at(1)),int(v.at(2)));
  if(id==0x42eff0){pause_=std::make_unique<PauseInventory>(globals_[0x46cd04]);if(target_)paused_frame_=*target_;return 0;}
  if(id==0x42d640||id==0x42fc60){spawn_shot(id==0x42d640?ProjectileKind::ball:ProjectileKind::water_bomb,int(v.at(4)),int(v.at(5)),int(v.at(6)));return 0;}
  // Original scheduler groups 6/7 hold transient entities and map processes.
  // Defer their destruction until update_player has returned.
  if(id==0x44d440){if(v.at(0)==6||v.at(0)==7)reload_pending_=true;return 0;}
  if(id==0x427900){reload_pending_=true;globals_[0x77bf4c]=globals_[0x77bf50]=0;return 0;}
  if(id==0x4321f0){reload_pending_=true;return 0;}
  if(id==0x43f1d0){return_to_menu_=true;return 0;}
  if(id==0x44d790){game_over_=true;game_over_started_=now_ms();if(play_music)play_music(14);return 0;}
  if(event)return event(id,p,args);
  throw std::runtime_error("unbound level event: "+std::to_string(id));
 }
 void tick(const InputState& input,IndexedSurface&target){
  player.sync_damage_immunity(now_ms(),bool(pause_)||subgame_active());
  const int old_player_x=player.at(8),old_player_y=player.at(12);
  if(return_to_menu_||complete_)return;
  if(!subgame_active())target.border_rgb.clear();
  if(scene_.stage==2&&!game_over_){
   ++ticks_;if(music_.update(now_ms())&&play_music)play_music(music_.track());
   FlyingStageMotion::camera(scene_.camera,player,int(scene_.map.width),int(scene_.map.height),global(0x461754)==1||flying_events_.stops_scroll());
   int ox=target.origin_x,oy=target.origin_y;
   target.origin_x=camera_x()-std::clamp(camera_x()-(target.width-640)/2,0,std::max(0,view_right_edge()-target.width));
   target.origin_y=camera_y()-std::clamp(camera_y()-(target.height-480)/2,0,std::max(0,int(scene_.map.height)*32-target.height));
  pan_scene_view(target);

   int vx=target.origin_x,vy=target.origin_y;target.clear();target.origin_x=target.origin_y=0;auto&bg=assets_.image("gra.app");
   for(int y=0;y<target.height;y+=32)for(int x=0;x<target.width;x+=32)target.blit(bg,x,y,false,0,0,32,32);
   draw_map_layers(target,scene_.map,assets_.image("stile.app"),camera_x()-vx,camera_y()-vy);target.origin_x=vx;target.origin_y=vy;
   flying_scenery_.tick(scene_.placements,camera_x(),camera_y(),assets_,target);
   for(std::size_t i=0;i<scene_.placements.size()&&!flying_events_.active();++i){auto&m=scene_.placements[i];if((m.type==0x4c||m.type==0x4d)&&!consumed_scene_placements_.count(i)&&FlyingStageMotion::player_hits(player,{0,-320,32,320},m.pixel_x(32),m.pixel_y(32))&&flying_events_.start(m.type,now_ms())){consumed_scene_placements_.insert(i);player.at(0xfc)=0;player.at(0xe8)=1;flying_.shots.clear();}}
   if(flying_events_.active()){flying_events_.tick(player,now_ms(),input.read(0x46d4ec)!=0,camera_x(),camera_y(),assets_,target,[&](unsigned id){sound(id);});draw_pickup_hud(assets_,player,target);companion_.tick(player,camera_x(),camera_y(),assets_,target,{});target.origin_x=ox;target.origin_y=oy;return;}
   // The map placement is consumed once, including enemies that leave the screen.
   for(std::size_t i=0;i<scene_.placements.size();++i){const auto&m=scene_.placements[i];
    if(spawned_[i]||m.cell_x<(camera_x()-vx)/32||m.cell_x>=(camera_x()-vx+target.width)/32+1||m.cell_y<(camera_y()-vy)/32||m.cell_y>=(camera_y()-vy+target.height)/32+1)continue;
    if(game::FlyingEnemyManager::supports(m.type)){const char*name=m.type==0x38?"shutbc.raw":m.type==0x39?"shutbe.raw":m.type==0x3c?"a.raw":m.type==0x3d?"b.raw":m.type==0x3e?"c.raw":m.type==0x3f?"d.raw":m.type==0x40?"e.raw":"shutba.raw";const auto&im=assets_.image(name);flying_enemies_.spawn(m.type,m.pixel_x(placement_sprite(m).width),m.pixel_y(placement_sprite(m).height),int(i));spawned_[i]=true;}
    else if(m.type==0x21){const auto&im=assets_.image("chh125bc.raw");flying_boss_=std::make_unique<FlyingBoss>(m.pixel_x(placement_sprite(m).width),m.pixel_y(placement_sprite(m).height),int(i));spawned_[i]=true;globals_[0x461754]=1;}
   }
   flying_enemies_.tick(camera_x(),camera_y(),player,assets_,target);
   for(auto&e:flying_enemies_.projectile_events){game::EntityPayload shot;shot.kind=e.kind;shot.at(4)=e.x;shot.at(8)=e.y;shot.at(0x3c)=e.vx;shot.at(0x40)=e.vy;flying_missiles_.push_back(shot);}flying_enemies_.projectile_events.clear();
   if(flying_boss_)flying_boss_->tick(camera_x(),camera_y(),player.at(8),player.at(12),player.at(0x10),assets_,target);
   flying_.tick(player,input,camera_x(),camera_y(),int(scene_.map.width),int(scene_.map.height),global(0x461754)==1,assets_,target);
   game::CollisionWorld flight_world(scene_.map,scene_.collision_blocks);flying_.collide_shots(flight_world);
   for(auto*e:flying_enemies_.targets()){game::CollisionRect r{e->at(0x20),e->at(0x24),e->at(0x28),e->at(0x2c)};
    for(auto&s:flying_.shots)if(e->at(0x50)!=4&&FlyingStageMotion::shot_hits(s,r,e->at(4),e->at(8))&&flying_enemies_.hurt(*e))award_hit();
    if(e->at(0x50)!=4&&FlyingStageMotion::player_hits(player,r,e->at(4),e->at(8))&&flying_.contact(player,*e)&&flying_enemies_.hurt(*e))award_hit();
   }
   if(flying_boss_&&!flying_boss_->completed){auto&e=flying_boss_->payload;for(auto&s:flying_.shots)if(FlyingStageMotion::shot_hits(s,FlyingBoss::rectangle(),e.at(4),e.at(8))&&flying_boss_->hurt())award_hit();
    if(FlyingStageMotion::player_hits(player,FlyingBoss::rectangle(),e.at(4),e.at(8))&&flying_.contact(player,e)&&flying_boss_->hurt())award_hit();
    for(auto&s:flying_boss_->projectiles)if(s.state!=2&&FlyingStageMotion::player_hits(player,FlyingStageMotion::shot_rectangle(),s.at(4),s.at(8)))flying_.contact(player,s);
   }
   for(auto&s:flying_missiles_){if(s.state==2)continue;if((s.at(4)<camera_x()+std::min(-640,-target.origin_x-128)||s.at(4)>camera_x()+std::max(1280,target.width-target.origin_x+128))||std::abs(s.at(8)-camera_y()-240)>std::max(720,target.height-target.origin_y+128)){s.state=2;continue;}target.blit(assets_.image(s.kind==0x24?"shutaa.raw":"shutfa.raw"),s.at(4)-camera_x(),s.at(8)-camera_y());s.at(4)+=s.at(0x3c);s.at(8)+=s.at(0x40);if(FlyingStageMotion::player_hits(player,FlyingStageMotion::shot_rectangle(),s.at(4),s.at(8)))flying_.contact(player,s);}
   flying_missiles_.erase(std::remove_if(flying_missiles_.begin(),flying_missiles_.end(),[](auto&s){return s.state==2;}),flying_missiles_.end());
   for(auto id:flying_.sounds)sound(id);flying_.sounds.clear();
   for(auto&file:flying_enemies_.sounds)if(play_sound_file)play_sound_file(file);flying_enemies_.sounds.clear();
   if(flying_boss_){for(auto&file:flying_boss_->sounds)if(play_sound_file)play_sound_file(file);flying_boss_->sounds.clear();if(flying_boss_->completed){complete_=true;globals_[0x46142c]=2;globals_[0x461428]=3;player.at(0xbc)=player.at(0xb4)=0;flying_missiles_.clear();flying_enemies_.clear();flying_.shots.clear();}}

   objects_->contact_override=[&](PlayerPayload&p,game::EntityPayload&e){if(e.kind==0x51&&e.at(0x3c)!=1){call(0x43c0a0,p,e,{});award_hit();++p.at(0x74);++globals_[0x45c330];}};
   objects_->tick(camera_x(),camera_y(),target,player,*this);
   draw_pickup_hud(assets_,player,target);companion_.tick(player,camera_x(),camera_y(),assets_,target,{});
   target.origin_x=ox;target.origin_y=oy;if(flying_.wants_menu)return_to_menu_=true;if(flying_.game_over){game_over_=true;game_over_started_=now_ms();if(play_music)play_music(14);}return;
  }
  if(global_ending_||ending_credits_){++ticks_;CutsceneFrame f=global_ending_?global_ending_->update(now_ms(),input.read(0x46d4e8)!=0):ending_credits_->update(input.read(0x46d4e8)!=0);draw_story_panel(target,f.draws);for(auto id:f.sounds)sound(id);if(f.cd_track&&play_music)play_music(f.cd_track);if(f.action==CutsceneFrame::credits){global_ending_.reset();ending_credits_=std::make_unique<EndingCredits>();}else if(f.action==CutsceneFrame::main_menu)return_to_menu_=true;return;}
  if(booster_ending_){++ticks_;auto f=booster_ending_->update(now_ms(),input.read(0x46d4e8)!=0);draw_story_panel(target,f.draws);for(auto id:f.sounds)sound(id);if(f.action==CutsceneFrame::resume_level){complete_=true;globals_[0x46142c]=3;globals_[0x461428]=4;player.at(0xbc)=player.at(0xb4)=0;}return;}
  if(later_exit_){++ticks_;auto f=later_exit_->update(now_ms(),input.read(0x46d4e8)!=0);draw_story_panel(target,f.draws);for(auto id:f.sounds)sound(id);
   if(f.action==CutsceneFrame::resume_level){complete_=true;globals_[0x46142c]=globals_[0x461428];++globals_[0x461428];player.at(0xbc)=player.at(0xb4)=0;}return;}

  if(extra_game_.active()){++ticks_;MinigameCanvas canvas(assets_,target);auto f=extra_game_.tick(now_ms(),{bool(input.read(0x46d4b0)),bool(input.read(0x46d4b4)),bool(input.read(0x46d4b8)),bool(input.read(0x46d4bc)),bool(input.read(0x46d4e8)),bool(input.read(0x46d4dc))},assets_,target);if(f.palette&&change_palette)change_palette(f.palette);if(f.cd==0&&stop_music)stop_music();for(auto&file:f.sounds)if(play_sound_file)play_sound_file(file);if(f.finished){for(unsigned a:{0x471928u,0x471938u,0x47193cu,0x471940u})globals_[a]=1;if(play_music)play_music(unsigned(scene_.stage+4));}return;}
  if(mole_game_.active()){++ticks_;MinigameCanvas canvas(assets_,target);auto f=mole_game_.tick(now_ms(),{bool(input.read(0x46d4b0)),bool(input.read(0x46d4b4)),bool(input.read(0x46d4b8)),bool(input.read(0x46d4bc)),bool(input.read(0x46d4e8)),bool(input.read(0x46d4dc))},assets_,target);if(f.palette&&change_palette)change_palette(f.palette);if(f.cd==0&&stop_music)stop_music();else if(f.cd>0&&play_music)play_music(unsigned(f.cd));for(auto&file:f.sounds)if(play_sound_file)play_sound_file(file);if(f.finished){for(unsigned a:{0x471928u,0x471938u,0x47193cu,0x471940u})globals_[a]=1;if(play_music)play_music(unsigned(scene_.stage+4));}return;}
  if(minigames_.active()){++ticks_;MinigameCanvas canvas(assets_,target);auto f=minigames_.tick(now_ms(),{input.read(0x46d4b0)!=0,input.read(0x46d4b4)!=0,input.read(0x46d4b8)!=0,input.read(0x46d4bc)!=0,input.read(0x46d4dc)!=0},assets_,target);
   if(f.loop_sound&&play_sound_loop)play_sound_loop(f.loop_sound);if(f.stop_sound&&stop_sound_file)stop_sound_file(f.stop_sound);
   if(f.palette&&change_palette)change_palette(f.palette);if(f.cd==0){if(stop_music)stop_music();}else if(f.cd>0&&play_music)play_music(unsigned(f.cd));for(auto&file:f.sounds)if(play_sound_file)play_sound_file(file);
   if(f.finished){for(unsigned a:{0x471928u,0x471938u,0x47193cu,0x471940u})globals_[a]=1;if(play_music)play_music(unsigned(scene_.stage+4));}return;
  }
  if(game_over_){++ticks_;target.blit(assets_.image("gmover.app"),0,0,false);bool space=input.read(0x46d4e8)!=0;if(space)game_over_space_=true;
   if((game_over_space_&&!space)||now_ms()-game_over_started_>=14001)return_to_menu_=true;return;}
  if(reload_pending_)reset_world();
  if(pause_){if(target.width==paused_frame_.width&&target.height==paused_frame_.height)target=paused_frame_;else{target.clear();auto ox=target.origin_x,oy=target.origin_y;target.origin_x=target.origin_y=0;auto&bg=assets_.image("gra.app");for(int yy=0;yy<target.height;yy+=32)for(int xx=0;xx<target.width;xx+=32){int sy=std::max(0,camera_y()-oy+yy-(int(scene_.map.height)-15)*32);target.blit(bg,xx,yy,false,0,sy,32,32);}draw_map_layers(target,scene_.map,assets_.image(game::stage_bindings()[scene_.stage].tiles),camera_x()-ox,camera_y()-oy);target.blit(paused_frame_,ox-paused_frame_.origin_x,oy-paused_frame_.origin_y,false);target.origin_x=ox;target.origin_y=oy;}if(pause_->tick(input,player,assets_,target)){
   int choice=pause_->choice();if(choice==4&&player.at(0xd0)>0&&globals_[0x46cda8]==0){companion_.activate();--player.at(0xd0);globals_[0x46cda8]=1;}
   else if(choice>=2&&choice!=4&&globals_[0x46cda8]==0){ensure_abilities();abilities_->activate(choice,player,camera_x(),camera_y());}
   globals_[0x46cd04]=choice;pause_.reset();}return;}
  ++ticks_;
  ensure_scene_callbacks();
  if(music_.update(now_ms())&&play_music)play_music(music_.track());
  const int spawn_left=std::clamp(camera_x()-(target.width-640)/2,0,std::max(0,view_right_edge()-target.width));
  const int spawn_top=std::clamp(camera_y()-(target.height-480)/2,0,std::max(0,int(scene_.map.height)*32-target.height));
  for(std::size_t i=0;i<scene_.placements.size();++i){auto&m=scene_.placements[i];if(spawned_[i]||m.cell_x<(spawn_left-64)/32||m.cell_x>=(spawn_left+target.width+64)/32+1||m.cell_y<(spawn_top-64)/32||m.cell_y>=(spawn_top+target.height+64)/32+1)continue;
   // Ordinary enemies activate across the complete visible world rectangle on every stage.
   // Keep Boss activation in its original corridor to preserve introductory sequences.
   bool boss_placement=m.type==0x12||m.type==32||m.type==0x25;
   if(boss_placement&&(m.cell_x<camera_x()/32||m.cell_x>=camera_x()/32+21||m.cell_y<camera_y()/32||m.cell_y>=camera_y()/32+16))continue;
   if(m.type>=0&&m.type<=2){const char* names[]={"c1_f.app","c2_f.app","c3_f.app"};auto&im=assets_.image(names[m.type]);enemies_->spawn(m.type,m.pixel_x(placement_sprite(m).width),m.pixel_y(placement_sprite(m).height),short(i));spawned_[i]=true;}
   else if(m.type==0x12){auto&im=assets_.image("ryoko_a2.app");boss_=std::make_unique<game::FirstBoss>(m.pixel_x(placement_sprite(m).width),m.pixel_y(placement_sprite(m).height),im.width,im.height);spawned_[i]=true;}
   else if(m.type==31||m.type==36||m.type==32||game::Stage2ExtraEnemy::supports(m.type)||m.type==0x25){ensure_later_actors();const char*name=m.type==31?"cht11cd.app":m.type==36?"cht122dd.app":m.type==0x25?"cht121bd.app":m.type==0x1d?"chch1bd.app":m.type==0x1e?"chch2cc.app":"cht144ac.app";auto&im=assets_.image(name);later_actors_->spawn(m.type,m.pixel_x(placement_sprite(m).width),m.pixel_y(placement_sprite(m).height),i);spawned_[i]=true;}
   else if(scene_.stage==3&&m.type==0x7a){auto&im=assets_.image("high2aa.raw");booster_boss_=std::make_unique<BoosterBoss>(m.pixel_x(placement_sprite(m).width),m.pixel_y(placement_sprite(m).height),assets_);spawned_[i]=true;}
   else if(game::LaterAdditionalEnemies::supports(m.type)){auto&im=assets_.image(game::LaterAdditionalEnemies::resource(m.type));additional_enemies_.spawn(m.type,m.pixel_x(placement_sprite(m).width),m.pixel_y(placement_sprite(m).height),int(i),assets_,scene_.stage==6&&m.type==0x77&&m.cell_x<100);spawned_[i]=true;}
   else if(game::LaterPatrolEnemy::supports(m.type)){auto&im=assets_.image(m.type==0x16?"chjg1aa.raw":(m.type==0x17||m.type==0x5d)?"chjg2aa.raw":"drflyaa.raw");patrol_enemies_.push_back({i,std::make_unique<game::LaterPatrolEnemy>(m.type,m.pixel_x(placement_sprite(m).width),m.pixel_y(placement_sprite(m).height),assets_)});spawned_[i]=true;}
   else if(game::LaterLargeGuard::supports(m.type)){auto&im=assets_.image(m.type==0x14?"chh111aa.raw":"chh211aa.raw");large_guards_.push_back({i,std::make_unique<game::LaterLargeGuard>(m.type,m.pixel_x(placement_sprite(m).width),m.pixel_y(placement_sprite(m).height),assets_)});spawned_[i]=true;}
   else if(m.type==0x79){auto&im=assets_.image("dol.raw");falling_dolls_.emplace_back(m.pixel_x(placement_sprite(m).width),m.pixel_y(placement_sprite(m).height));spawned_[i]=true;}
   else if(LaterSprings::supports(m.type)){auto&im=assets_.image(LaterSprings::resource(m.type));later_springs_.spawn(m.type,m.pixel_x(placement_sprite(m).width),m.pixel_y(placement_sprite(m).height),int(i));spawned_[i]=true;}
   else if(LaterMechanisms::supports(m.type)){auto&im=assets_.image(LaterMechanisms::resource(m.type));later_mechanisms_.spawn(m.type,m.pixel_x(placement_sprite(m).width),m.pixel_y(placement_sprite(m).height),int(i));spawned_[i]=true;}
   else if(game::LaterSpecialEnemies::supports(m.type)){auto&im=assets_.image(game::LaterSpecialEnemies::placement_image(m.type));special_enemies_.spawn(m.type,m.pixel_x(placement_sprite(m).width),m.pixel_y(placement_sprite(m).height),int(i));spawned_[i]=true;}
   else if(m.type==0x5f){auto&im=assets_.image("fire1.raw");fire_traps_.spawn(int(i),m.pixel_x(placement_sprite(m).width),m.pixel_y(placement_sprite(m).height));fire_traps_.objects.back().e.at(0xc)=im.width;fire_traps_.objects.back().e.at(0x10)=im.height;spawned_[i]=true;}
   else if(m.type==0x5c){fall_.start(player);spawned_[i]=true;}
   else if(m.type==0x23){auto&im=assets_.image("hw6.app");emitters_.add(m.pixel_x(placement_sprite(m).width),m.pixel_y(placement_sprite(m).height));spawned_[i]=true;}
   else if(Level1SceneObjects::supports(m.type)){int w=32,h=32;if(m.type==0x57){w=79;h=91;}else if(m.type==0x7b){w=73;h=102;}else if(m.type==0x7c){w=58;h=104;}if(m.type==0x7d||m.type==0x7e){auto&im=assets_.image(m.type==0x7d?"gamepuzl.raw":"gamecard.raw");w=im.width;h=im.height;}scene_objects_.spawn(m.type,m.pixel_x(w),m.pixel_y(h),int(i));spawned_[i]=true;}}
  // MAP type 0x5a creates 44d540: after 20 group-6 updates enable horizontal integration.
  for(std::size_t i=0;i<scene_.placements.size();++i){auto&m=scene_.placements[i];if(!spawned_[i]&&m.type==0x5a&&m.cell_x>=camera_x()/32&&m.cell_x<camera_x()/32+21&&m.cell_y>=camera_y()/32-1&&m.cell_y<camera_y()/32+17){spawned_[i]=true;movement_enable_ticks_=0;}}
  if(movement_enable_ticks_>=0&&++movement_enable_ticks_==20){player.at(0x100)=1;movement_enable_ticks_=-1;}
  update_later_sequence(input.read(0x46d4ec)!=0,true);
  if(!later_sequence_.active()||!later_sequence_.camera_frozen)game::begin_camera_frame(scene_.camera,int(scene_.map.width),int(scene_.map.height));
  if(!later_sequence_.active()||!later_sequence_.camera_frozen)game::plan_normal_camera(scene_.camera,player,view_right_edge()/32,int(scene_.map.height));
  struct RestoreOrigin{IndexedSurface&surface;int x,y;~RestoreOrigin(){surface.origin_x=x;surface.origin_y=y;}} restore_origin{target,target.origin_x,target.origin_y};
  // Keep original gameplay/camera coordinates, but bound the expanded view
  // to the actual map. At the left edge, extra width reveals world to the right.
  target.origin_x=camera_x()-std::clamp(camera_x()-(target.width-640)/2,0,std::max(0,view_right_edge()-target.width));
  target.origin_y=camera_y()-std::clamp(camera_y()-(target.height-480)/2,0,std::max(0,int(scene_.map.height)*32-target.height));
  pan_scene_view(target);

  target_=&target;for(unsigned a=0x46d4b0;a<=0x46d4f0;a+=4)globals_[a]=int(input.read(a));globals_[0x46d4ac]=1;
  target.clear();int view_origin_x=target.origin_x,view_origin_y=target.origin_y;target.origin_x=target.origin_y=0;auto&bg=assets_.image("gra.app");
  for(int y=0;y<target.height;y+=32)for(int x=0;x<target.width;x+=32){int sy=std::max(0,camera_y()-view_origin_y+y-(int(scene_.map.height)-15)*32);target.blit(bg,x,y,false,0,sy,32,32);}
  draw_map_layers(target,scene_.map,assets_.image(game::stage_bindings()[scene_.stage].tiles),camera_x()-view_origin_x,camera_y()-view_origin_y);
  target.origin_x=view_origin_x;target.origin_y=view_origin_y;
  // fire1..3 include an opaque 192x200 wall patch: draw behind every actor.
  fire_traps_.tick(camera_x(),camera_y(),assets_,target);
  // MAP74 / 414210 draws the permanent urn, independently of its one-shot trigger.
  for(const auto&o:fixed_scenery_)target.blit(assets_.image(o.resource),o.x-camera_x(),o.y-camera_y());
  if(sixth_scenes_.active()){game::CollisionWorld world(scene_.map,scene_.collision_blocks);sixth_scenes_.tick(now_ms(),input.read(0x46d4ec)!=0,player,camera_x(),camera_y(),assets_,target,world,[&](unsigned id){sound(id);});draw_pickup_hud(assets_,player,target);companion_.tick(player,camera_x(),camera_y(),assets_,target,{});if(sixth_scenes_.advance_stage_requested){complete_=true;globals_[0x46142c]=5;globals_[0x461428]=6;}return;}
  if(final_encounter_.active()){game::CollisionWorld world(scene_.map,scene_.collision_blocks);final_encounter_.tick(now_ms(),player,camera_x(),camera_y(),assets_,target,world,[&](unsigned id){sound(id);},play_music);draw_pickup_hud(assets_,player,target);companion_.tick(player,camera_x(),camera_y(),assets_,target,{});return;}
  if(jungle_intro_.active()){game::CollisionWorld world(scene_.map,scene_.collision_blocks);jungle_intro_.tick(now_ms(),player,*jungle_bosses_[0],*jungle_bosses_[1],camera_x(),camera_y(),assets_,target,world,[&](unsigned id){sound(id);});draw_pickup_hud(assets_,player,target);companion_.tick(player,camera_x(),camera_y(),assets_,target,{});return;}
  enemies_->tick(camera_x(),camera_y(),target);
  if(later_actors_){later_actors_->tick(camera_x(),camera_y(),target,now_ms());for(auto&r:later_actors_->respawn_records)relocate_enemy(r.placement,r.x,r.y);later_actors_->respawn_records.clear();later_actors_->respawn_indices.clear();for(auto&file:later_actors_->sounds)if(play_sound_file)play_sound_file(file);later_actors_->sounds.clear();}
  update_later_sequence(input.read(0x46d4ec)!=0,false);if(later_exit_)return;
  if(!enemy_missiles_)enemy_missiles_=std::make_unique<EnemyMissiles>(assets_,scene_.map,scene_.collision_blocks);
  enemy_missiles_->tick(camera_x(),camera_y(),target);if(later_projectiles_)later_projectiles_->tick(camera_x(),camera_y(),target);if(flame_projectiles_)flame_projectiles_->tick(camera_x(),camera_y(),target);
  if(later_actors_){for(auto&p:later_actors_->projectiles){if(p.callback==0x402440){if(!later_projectiles_)later_projectiles_=std::make_unique<game::Stage2Projectiles>(assets_,scene_.map,scene_.collision_blocks);later_projectiles_->spawn(p.x,p.y,p.vx,p.vy);}else if(p.callback==0x44a130){if(!flame_projectiles_)flame_projectiles_=std::make_unique<game::Stage2ExtraProjectiles>(assets_,scene_.map,scene_.collision_blocks);flame_projectiles_->spawn(p.x,p.y,p.vx,p.vy);}else if(p.callback==0x4366b0)enemy_missiles_->spawn(0x5b,p.x,p.y,p.vx,p.vy);else throw std::runtime_error("unknown later-stage projectile");}later_actors_->projectiles.clear();}
  for(auto&m:enemies_->projectiles)enemy_missiles_->spawn(m.type,m.x,m.y,m.vx,m.vy);
  enemies_->projectiles.clear();
  for(auto&s:enemies_->respawns)if(s.map_tag>=0)relocate_enemy(std::size_t(s.map_tag),s.x,s.y);
  enemies_->respawns.clear();
  auto dialogue_actor_collision=[&](Level1Dialogues::Actor&a){game::CollisionWorld world(scene_.map,scene_.collision_blocks);Level1Dialogues::collide_actor(a,world);};
  if(scene_events_.active())scene_events_.tick(player,now_ms(),bool(input.read(0x46d4ec)),camera_x(),camera_y(),assets_,target,[&](const char*f){if(play_sound_file)play_sound_file(f);},{});
  else if(later_monologues_.active())later_monologues_.tick(player,now_ms(),bool(input.read(0x46d4ec)),camera_x(),camera_y(),assets_,target,[&](unsigned id){sound(id);});
  else if(later_player_.active()){game::CollisionWorld world(scene_.map,scene_.collision_blocks);later_player_.tick(player,world,camera_x(),camera_y(),assets_,target,false);}
  else if(fall_.active()){game::CollisionWorld world(scene_.map,scene_.collision_blocks);fall_.tick(player,world,camera_x(),camera_y(),assets_,target);}
  else if(dialogues_.active())dialogues_.tick(player,now_ms(),input.read(0x46d4ec)!=0,camera_x(),camera_y(),assets_,target,[&](const char*f){if(play_sound_file)play_sound_file(f);},{},dialogue_actor_collision);
  else {if(gate_.active())gate_.tick(player,now_ms(),input.read(0x46d4ec)!=0,camera_x(),camera_y(),assets_,target,play_sound_file);else if(scene_.stage==3)booster_.tick(player,input,camera_x(),camera_y(),int(scene_.map.width),int(scene_.map.height),assets_,target,flying_);else update_player(player,*this);dialogues_.tick_actors(camera_x(),camera_y(),assets_,target,dialogue_actor_collision);}
  if(game_over_||return_to_menu_||reload_pending_)return;
  if(gate_.gate_removed)for(auto&o:objects_->objects())if(o.type==0x76)o.removed=true;
  if(gate_.completed&&!gate_rewarded_){gate_rewarded_=true;award_hit();}
  for(auto&shot:shots_){if(shot->owner_state==1){shot->owner_state=0;continue;}if(shot->owner_state==0)update_projectile(*shot,*this);}
  game::CollisionWorld world(scene_.map,scene_.collision_blocks);game::CollisionRect rect={player.at(0x2c),player.at(0x30),player.at(0x34),player.at(0x38)};
  int material=world.body(rect,player.at(8),player.at(12));if(material){auto r=game::respond_player_world(player,world,material);if(r.hurt_sound_requested){if(!play_sound_file)throw std::runtime_error("unbound damage sound");play_sound_file("RESOURCE/dem-shin.wav");}if(r.safety_limit_reached)throw std::runtime_error("terrain response budget exhausted");}
  for(auto&shot:shots_)if(shot->owner_state==0){int hit=world.body({shot->at(0x14),shot->at(0x18),shot->at(0x1c),shot->at(0x20)},shot->at(4),shot->at(8));if(hit)projectile_terrain_collision(*shot,hit,*this);}
  if(later_actors_)for(auto*e:later_actors_->targets())if(game::intersects(rect,player.at(8),player.at(12),game::Stage2Actors::rectangle(*e),e->at(4),e->at(8)))game::respond_player_object(player,*e,*this);
  for(auto&o:fire_traps_.objects)if(o.e.state==0&&game::intersects(rect,player.at(8),player.at(12),LaterFireTraps::rectangle(),o.e.at(4),o.e.at(8)))game::respond_player_object(player,o.e,*this);
  for(auto&r:fire_traps_.respawns)if(r.index>=0&&std::size_t(r.index)<spawned_.size())spawned_[r.index]=false;
  fire_traps_.respawns.clear();fire_traps_.objects.erase(std::remove_if(fire_traps_.objects.begin(),fire_traps_.objects.end(),[](auto&o){return o.e.state==2;}),fire_traps_.objects.end());
  emitters_.tick(camera_x(),camera_y(),assets_,target);falling_hazards_.tick(camera_x(),camera_y(),assets_,target,player,*this);
  objects_->tick(camera_x(),camera_y(),target,player,*this);
  if(later_actors_)for(auto*e:later_actors_->targets())for(auto&o:objects_->objects())if(!o.removed&&o.payload.kind==63&&game::intersects(game::Stage2Actors::rectangle(*e),e->at(4),e->at(8),{0,0,15,15},o.payload.at(4),o.payload.at(8)))later_actors_->platform(*e,o.payload.at(0x40)&255);
  if(!springs_)springs_=std::make_unique<Level1Springs>(assets_,scene_.placements);
  springs_->tick(camera_x(),camera_y(),target,player,*this,old_player_x,old_player_y);
  for(auto*enemy:enemies_->targets())for(auto&o:objects_->objects())if(!o.removed&&o.payload.kind==63&&game::intersects({28,14,46,71},enemy->x,enemy->y,{0,0,15,15},o.payload.at(4),o.payload.at(8)))enemies_->platform(*enemy,o.payload.at(0x40)&255);
  for(std::size_t i=0;i<scene_.placements.size()&&!scene_events_.active();++i){auto&m=scene_.placements[i];if(spawned_[i]||consumed_scene_placements_.count(i)||consumed_scene_types_.count(m.type)||!Stage2SceneEvents::supports(m.type))continue;if(game::intersects(rect,player.at(8),player.at(12),{0,-320,32,320},m.pixel_x(32),m.pixel_y(32))&&scene_events_.start(m.type,player,now_ms(),[&](const char*f){if(play_sound_file)play_sound_file(f); })){spawned_[i]=true;consumed_scene_placements_.insert(i);consumed_scene_types_.insert(m.type);}}
  if(scene_.stage>=4)for(std::size_t i=0;i<scene_.placements.size()&&!later_monologues_.active();++i){auto&m=scene_.placements[i];if(!LaterMonologues::supports(m.type)||consumed_scene_types_.count(m.type))continue;if(game::intersects(rect,player.at(8),player.at(12),{0,-320,32,320},m.pixel_x(32),m.pixel_y(32))&&later_monologues_.start(m.type,player,now_ms(),[&](unsigned id){sound(id);})){consumed_scene_types_.insert(m.type);consumed_scene_placements_.insert(i);}}
  scene_objects_.tick(camera_x(),camera_y(),assets_,target,player,*this,[&](unsigned constructor,unsigned,unsigned,unsigned){dialogues_.start(constructor==0x443cd0?0x41:0x75,player,now_ms(),camera_x(),target.width);});
  if(subgame_active())return; // group5 subgame froze groups1/6/7 at the contact callback.
  if(flame_projectiles_)for(auto&e:flame_projectiles_->targets)if(e.state==0&&game::intersects(rect,player.at(8),player.at(12),{10,10,40,40},e.at(4),e.at(8)))game::respond_player_object(player,e,*this);
  if(later_projectiles_)for(auto&e:later_projectiles_->targets)if(e.state==0&&game::intersects(rect,player.at(8),player.at(12),{2,2,12,12},e.at(4),e.at(8)))game::respond_player_object(player,e,*this);
  for(auto&m:enemy_missiles_->missiles){auto&p=m.payload;if(p.owner_state==2)continue;
   if(game::intersects(rect,player.at(8),player.at(12),{p.at(0x14),p.at(0x18),p.at(0x1c),p.at(0x20)},p.at(4),p.at(8))){
    game::EntityPayload e;e.kind=m.kind;for(unsigned o=4;o<=0x40;o+=4)e.at(o)=p.at(o);
    game::respond_player_object(player,e,*this);if(e.state==2)p.owner_state=2;
   }
  }
  auto&missiles=enemy_missiles_->missiles;missiles.erase(std::remove_if(missiles.begin(),missiles.end(),[](const EnemyMissile&m){return m.payload.owner_state==2;}),missiles.end());
  for(auto*enemy:enemies_->targets()){game::EntityPayload e;e.kind=0x65+enemy->map_type;e.at(4)=enemy->x;e.at(8)=enemy->y;e.at(0xc)=e.at(0x64)=e.at(0x6c)=enemy->width;e.at(0x24)=71;e.at(enemy->map_type==0?0x58:0x50)=enemy->state;
   if(game::intersects(rect,player.at(8),player.at(12),{28,14,46,71},enemy->x,enemy->y)){contact_enemy_=enemy;game::respond_player_object(player,e,*this);contact_enemy_=nullptr;}}
  for(auto&f:enemies_->sound_requests){if(play_sound_file)play_sound_file(f);}enemies_->sound_requests.clear();
  if(boss_&&boss_->payload.state!=2){
   auto result=boss_->update(player.at(8),player.at(0x10));
   // 4220a0 returns immediately when the death animation advances the stage.
   // Removed bosses must not participate in another contact or damage pass.
   if(result.completed){complete_=true;++globals_[0x45c334];globals_[0x46142c]=globals_[0x461428];++globals_[0x461428];player.at(0xbc)=player.at(0xb4)=0;return;}
   auto&e=boss_->payload;game::CollisionRect br={e.at(0x20),e.at(0x24),e.at(0x28),e.at(0x2c)};
   int m=world.body(br,e.at(4),e.at(8));if(m)boss_->terrain(world,m);boss_->render(assets_,target,camera_x(),camera_y());
   if(game::intersects(rect,player.at(8),player.at(12),br,e.at(4),e.at(8)))game::respond_player_object(player,e,*this);
  }
  if(scene_.stage==5&&!sixth_scenes_.active())for(auto&m:scene_.placements)if((m.type==0x6f||m.type==0x70)&&!consumed_scene_types_.count(m.type)&&game::intersects(rect,player.at(8),player.at(12),{0,-320,32,320},m.pixel_x(32),m.pixel_y(32))){consumed_scene_types_.insert(m.type);sixth_scenes_.begin(m.type,now_ms(),player,[&](unsigned id){sound(id);});return;}
  if(scene_.stage==6&&!consumed_scene_types_.count(0x74))for(auto&m:scene_.placements)if(m.type==0x74&&game::intersects(rect,player.at(8),player.at(12),{-80,-320,32,320},m.pixel_x(assets_.image("hang.raw").width),m.pixel_y(assets_.image("hang.raw").height))){consumed_scene_types_.insert(0x74);globals_[0x46c500]=2;player.at(0xfc)=0;final_encounter_.begin(now_ms());return;}
  if(final_encounter_.completed()&&final_encounter_.robot&&final_encounter_.nurse){int dead=0;for(auto*b:{final_encounter_.robot.get(),final_encounter_.nurse.get()}){if(b->defeated){++dead;continue;}b->tick(camera_x(),camera_y(),player.at(8),player.at(12),player.at(0x10),assets_,target);if(b->defeated){++dead;continue;}auto&e=b->payload;auto r=b->rectangle();int material=world.body(r,e.at(4),e.at(8));if(material)b->terrain(world,material);if(!b->dying()&&game::intersects(rect,player.at(8),player.at(12),r,e.at(4),e.at(8)))game::respond_player_object(player,e,*this);for(auto&s:shots_)if(s->owner_state==0&&game::intersects({s->at(0x14),s->at(0x18),s->at(0x1c),s->at(0x20)},s->at(4),s->at(8),r,e.at(4),e.at(8))&&b->hurt()){s->owner_state=2;award_hit();}for(auto&request:b->projectiles)final_missiles_.spawn(request.constructor,request.x,request.y,request.vx);b->projectiles.clear();for(auto&file:b->sounds)if(play_sound_file)play_sound_file(file);b->sounds.clear();}globals_[0x46c500]=2-dead;if(dead==2){global_ending_=std::make_unique<GlobalEnding>(now_ms());globals_[0x46c500]=2;return;}}
  final_missiles_.tick(camera_x(),camera_y(),assets_,target,world);for(auto&s:final_missiles_.shots)if(s.e.state!=2&&game::intersects(rect,player.at(8),player.at(12),{s.e.at(0x14),s.e.at(0x18),s.e.at(0x1c),s.e.at(0x20)},s.e.at(4),s.e.at(8)))game::respond_player_object(player,s.e,*this);
  if(scene_.stage==4&&!consumed_scene_types_.count(0x56))for(auto&m:scene_.placements)if(m.type==0x56&&game::intersects(rect,player.at(8),player.at(12),{0,-320,32,320},m.pixel_x(32),m.pixel_y(32))){jungle_bosses_.push_back(std::make_unique<JungleBoss>(0x27,player.at(8)+340,player.at(12)-200,assets_));jungle_bosses_.push_back(std::make_unique<JungleBoss>(0x28,player.at(8)+400,player.at(12)-200,assets_));globals_[0x45c0a8]=2;consumed_scene_types_.insert(0x56);jungle_intro_.begin(now_ms(),player,[&](unsigned id){sound(id);});return;}
  if(!jungle_bosses_.empty()){int dead=0;for(auto&b:jungle_bosses_){if(b->defeated){++dead;continue;}b->tick(camera_x(),camera_y(),player.at(8),player.at(0x10),assets_,target);if(b->defeated){++dead;continue;}auto&e=b->payload;auto r=b->rectangle();int material=world.body(r,e.at(4),e.at(8));if(material)b->terrain(world,material);if(!b->dying()&&game::intersects(rect,player.at(8),player.at(12),r,e.at(4),e.at(8)))game::respond_player_object(player,e,*this);for(auto&s:shots_)if(s->owner_state==0&&game::intersects({s->at(0x14),s->at(0x18),s->at(0x1c),s->at(0x20)},s->at(4),s->at(8),r,e.at(4),e.at(8))&&b->hurt()){s->owner_state=2;award_hit();}for(auto&request:b->projectiles)jungle_missiles_.spawn(request.x,request.y,request.vx);b->projectiles.clear();for(auto&file:b->sounds)if(play_sound_file)play_sound_file(file);b->sounds.clear();}globals_[0x45c0a8]=2-dead;if(dead==2){complete_=true;globals_[0x46142c]=4;globals_[0x461428]=5;globals_[0x45c0a8]=2;return;}}
  jungle_missiles_.tick(camera_x(),camera_y(),assets_,target,world);for(auto&s:jungle_missiles_.shots)if(s.e.state!=2&&game::intersects(rect,player.at(8),player.at(12),{0,0,10,10},s.e.at(4),s.e.at(8)))game::respond_player_object(player,s.e,*this);
  if(booster_boss_){auto&b=*booster_boss_;auto&e=b.payload;b.tick(camera_x(),camera_y(),player.at(8),player.at(0x10),assets_,target);if(e.state!=2){int material=world.body(BoosterBoss::rectangle(),e.at(4),e.at(8));if(material)b.terrain(world,material);for(auto&s:flying_.shots)if(FlyingStageMotion::shot_hits(s,BoosterBoss::rectangle(),e.at(4),e.at(8))&&b.hurt())award_hit();if(game::intersects(rect,player.at(8),player.at(12),BoosterBoss::rectangle(),e.at(4),e.at(8))&&booster_.hurt(player))sound(0x4752e8);}for(auto&r:b.projectiles)booster_missiles_.spawn(r.x,r.y,r.vx,r.delay);b.projectiles.clear();for(auto&file:b.sounds)if(play_sound_file)play_sound_file(file);b.sounds.clear();if(b.ending_requested&&!booster_ending_)booster_ending_=std::make_unique<BoosterEnding>();}
  if(scene_.stage==3){booster_missiles_.tick(camera_x(),camera_y(),player.at(12),assets_,target,world);for(auto&s:booster_missiles_.shots)if(game::intersects(rect,player.at(8),player.at(12),{0,0,10,10},s.e.at(4),s.e.at(8))&&booster_.hurt(player)){sound(0x4752e8);s.e.state=2;}}
  for(auto&entry:large_guards_){auto&a=*entry.actor;auto&e=a.payload;if(e.state==2)continue;a.tick(camera_x(),camera_y(),assets_,target);if(e.state==2)continue;auto r=game::LaterLargeGuard::rectangle(e);for(auto&o:objects_->objects())if(!o.removed&&o.payload.kind==0x3f&&game::intersects(r,e.at(4),e.at(8),{0,0,15,15},o.payload.at(4),o.payload.at(8)))a.trigger(o.payload.at(0x40)&255);int material=world.body(r,e.at(4),e.at(8));if(material)a.terrain(world,material);if(e.at(0x58)!=4&&game::intersects(rect,player.at(8),player.at(12),r,e.at(4),e.at(8))){if(scene_.stage==3){if(booster_.hurt(player))sound(0x4752e8);}else game::respond_player_object(player,e,*this);}for(auto&s:flying_.shots)if(e.at(0x58)!=4&&FlyingStageMotion::shot_hits(s,r,e.at(4),e.at(8))){a.hurt();award_hit();}for(auto&s:shots_)if(s->owner_state==0&&e.at(0x58)!=4&&game::intersects({s->at(0x14),s->at(0x18),s->at(0x1c),s->at(0x20)},s->at(4),s->at(8),r,e.at(4),e.at(8))){a.hurt();s->owner_state=2;award_hit();}for(auto&file:a.sound_requests)if(play_sound_file)play_sound_file(file);a.sound_requests.clear();}
  for(auto&entry:patrol_enemies_){auto&a=*entry.actor;auto&e=a.payload;if(e.state==2)continue;a.tick(camera_x(),camera_y(),assets_,target,&player);if(e.state==2)continue;auto r=game::LaterPatrolEnemy::rectangle(e);for(auto&o:objects_->objects())if(!o.removed&&o.payload.kind==0x3f&&game::intersects(r,e.at(4),e.at(8),{0,0,15,15},o.payload.at(4),o.payload.at(8)))a.trigger(o.payload.at(0x40)&255);int material=world.body(r,e.at(4),e.at(8));if(material)a.terrain(world,material);int dying=e.kind==0x7b?3:4;if(e.at(0x58)!=dying&&game::intersects(rect,player.at(8),player.at(12),r,e.at(4),e.at(8)))game::respond_player_object(player,e,*this);for(auto&s:shots_)if(s->owner_state==0&&e.at(0x58)!=dying&&game::intersects({s->at(0x14),s->at(0x18),s->at(0x1c),s->at(0x20)},s->at(4),s->at(8),r,e.at(4),e.at(8))){a.hurt();s->owner_state=2;award_hit();}for(auto&request:a.projectile_events)yaja_shots_.spawn(request.x,request.y,request.vx,request.vy);a.projectile_events.clear();for(auto&file:a.sound_requests)if(play_sound_file)play_sound_file(file);a.sound_requests.clear();}
  for(auto&r:later_mechanisms_.respawns){auto it=std::find_if(later_mechanisms_.objects.begin(),later_mechanisms_.objects.end(),[&](auto&o){return o.index==r.index&&o.e.state==2;});if(it!=later_mechanisms_.objects.end()){auto&im=assets_.image(LaterMechanisms::resource(it->type));rearm_scenery(r.index,r.type,r.x,r.y,im.width,im.height);}}later_mechanisms_.respawns.clear();later_mechanisms_.objects.erase(std::remove_if(later_mechanisms_.objects.begin(),later_mechanisms_.objects.end(),[](auto&o){return o.e.state==2;}),later_mechanisms_.objects.end());
  for(auto&r:later_springs_.respawns){auto&im=assets_.image(LaterSprings::resource(r.type));rearm_scenery(r.index,r.type,r.x,r.y,im.width,im.height);}later_springs_.respawns.clear();later_springs_.objects.erase(std::remove_if(later_springs_.objects.begin(),later_springs_.objects.end(),[](auto&o){return o.e.state==2;}),later_springs_.objects.end());
  additional_enemies_.tick(camera_x(),camera_y(),player,assets_,target,world,play_sound_file);for(auto*e:additional_enemies_.targets()){auto r=game::LaterAdditionalEnemies::rectangle(*e);for(auto&o:objects_->objects())if(!o.removed&&o.payload.kind==0x3f&&game::intersects(r,e->at(4),e->at(8),{0,0,15,15},o.payload.at(4),o.payload.at(8)))additional_enemies_.trigger(*e,o.payload.at(0x40)&255);if(game::intersects(rect,player.at(8),player.at(12),r,e->at(4),e->at(8)))game::respond_player_object(player,*e,*this);for(auto&s:shots_)if(s->owner_state==0&&game::intersects({s->at(0x14),s->at(0x18),s->at(0x1c),s->at(0x20)},s->at(4),s->at(8),r,e->at(4),e->at(8))&&additional_enemies_.hurt(*e)){s->owner_state=2;award_hit();}}
  for(auto&d:falling_dolls_){
   d.tick(camera_x(),camera_y(),assets_,target,now_ms());
   if(d.updated_this_tick&&d.e.state==0){int material=world.body(LaterFallingDoll::rectangle(),d.e.at(4),d.e.at(8));if(material)d.terrain(world,material);}
   // Contact remains live between stone steps, while terrain rebounds run once.
   if(d.active_this_tick&&d.e.state==0&&game::intersects(rect,player.at(8),player.at(12),LaterFallingDoll::rectangle(),d.e.at(4),d.e.at(8)))game::respond_player_object(player,d.e,*this);
   for(auto id:d.sounds)sound(globals_[0x473920]++%2?0x473924:id);d.sounds.clear();
  }
  // Safety activation follows the player's swept corridor, not a lagging vertical camera.
  for(std::size_t i=0;i<scene_.placements.size();++i){auto&m=scene_.placements[i];if(spawned_[i]||!LaterSprings::supports(m.type))continue;if(m.cell_x*32<std::min(old_player_x,player.at(8))-96||m.cell_x*32>std::max(old_player_x,player.at(8))+128||m.cell_y*32<std::min(old_player_y,player.at(12))-128||m.cell_y*32>std::max(old_player_y,player.at(12))+160)continue;auto&im=assets_.image(LaterSprings::resource(m.type));later_springs_.spawn(m.type,m.pixel_x(placement_sprite(m).width),m.pixel_y(placement_sprite(m).height),int(i));spawned_[i]=true;}
  later_springs_.tick(camera_x(),camera_y(),assets_,target);for(auto*e:later_springs_.targets()){if(spring_swept_contact(player,*e,*this,old_player_x,old_player_y,e->at(0x40),e->at(0x44)))continue;if(game::intersects(rect,player.at(8),player.at(12),{0,0,56,10},e->at(4),e->at(8)))game::respond_player_object(player,*e,*this);}
  yaja_shots_.tick(camera_x(),camera_y(),assets_,target,world);for(auto&e:yaja_shots_.targets)if(e.state!=2&&game::intersects(rect,player.at(8),player.at(12),game::LaterYajaProjectiles::rectangle(),e.at(4),e.at(8)))game::respond_player_object(player,e,*this);
  if(scene_.stage>=3){later_mechanisms_.tick(camera_x(),camera_y(),assets_,target);special_enemies_.tick(camera_x(),camera_y(),player,assets_,target);
   for(auto*e:later_mechanisms_.targets()){int material=world.body(LaterMechanisms::rectangle(),e->at(4),e->at(8));if(material)later_mechanisms_.terrain(*e,world,material);for(auto&o:objects_->objects())if(!o.removed&&o.payload.kind==0x3f&&game::intersects(LaterMechanisms::rectangle(),e->at(4),e->at(8),{0,0,15,15},o.payload.at(4),o.payload.at(8)))later_mechanisms_.trigger(*e,o.payload.at(0x40)&255);if(game::intersects(rect,player.at(8),player.at(12),LaterMechanisms::rectangle(),e->at(4),e->at(8))){later_mechanisms_.contact(*e);game::respond_player_object(player,*e,*this);}}
   for(auto*e:special_enemies_.targets()){auto r=game::LaterSpecialEnemies::rectangle(*e);for(auto&o:objects_->objects())if(!o.removed&&o.payload.kind==0x3f&&game::intersects(r,e->at(4),e->at(8),{0,0,15,15},o.payload.at(4),o.payload.at(8)))special_enemies_.trigger(*e,o.payload.at(0x40)&255);int material=world.body(r,e->at(4),e->at(8));if(material)special_enemies_.terrain(*e,world,material);if(game::intersects(rect,player.at(8),player.at(12),r,e->at(4),e->at(8)))game::respond_player_object(player,*e,*this);for(auto&s:shots_)if(s->owner_state==0&&game::intersects({s->at(0x14),s->at(0x18),s->at(0x1c),s->at(0x20)},s->at(4),s->at(8),r,e->at(4),e->at(8))&&special_enemies_.hurt(*e)){s->owner_state=2;award_hit();}}
   for(auto&request:special_enemies_.projectile_events)enemy_missiles_->spawn(request.kind,request.x,request.y,request.vx,request.vy);special_enemies_.projectile_events.clear();for(auto&file:special_enemies_.sounds)if(play_sound_file)play_sound_file(file);special_enemies_.sounds.clear();
  }
  for(auto&shot:shots_)if(shot->owner_state==0&&boss_&&boss_->payload.state==0&&boss_->payload.at(0x50)!=4){auto&e=boss_->payload;
    if(game::intersects({shot->at(0x14),shot->at(0x18),shot->at(0x1c),shot->at(0x20)},shot->at(4),shot->at(8),{e.at(0x20),e.at(0x24),e.at(0x28),e.at(0x2c)},e.at(4),e.at(8))){sound(boss_->hurt());award_hit();shot->owner_state=2;queue_water_explosion(*shot);}}
  if(scene_.stage==3){flying_.collide_shots(world);for(auto&s:flying_.shots)if(s.state!=2){bool hit=enemies_->hit(FlyingStageMotion::shot_rectangle(),s.at(4),s.at(8));if(!hit&&later_actors_)hit=later_actors_->hit(FlyingStageMotion::shot_rectangle(),s.at(4),s.at(8));if(hit){s.state=2;award_hit();}}if(flying_.game_over){game_over_=true;game_over_started_=now_ms();if(play_music)play_music(14);}}
  for(auto&shot:shots_)if(shot->owner_state==0&&later_actors_&&later_actors_->hit({shot->at(0x14),shot->at(0x18),shot->at(0x1c),shot->at(0x20)},shot->at(4),shot->at(8))){award_hit();shot->owner_state=2;queue_water_explosion(*shot);}
  for(auto&shot:shots_)if(shot->owner_state==0&&enemies_->hit({shot->at(0x14),shot->at(0x18),shot->at(0x1c),shot->at(0x20)},shot->at(4),shot->at(8))){award_hit();shot->owner_state=2;queue_water_explosion(*shot);}
  for(auto&shot:shots_)if(shot->owner_state==2)queue_water_explosion(*shot);
  for(auto&ex:explosions_){if(ex.fresh){ex.fresh=false;continue;}if(std::abs(ex.x-camera_x()-320)>=961)continue;
   target.blit(assets_.image("boom.app"),ex.x-camera_x(),ex.y-camera_y());
   for(auto*enemy:enemies_->targets())if(enemy->state!=(enemy->map_type==0?3:4)&&game::intersects({14,12,106,70},ex.x,ex.y,{28,14,46,71},enemy->x,enemy->y)){enemies_->defeat(*enemy);award_hit();}
   if(boss_&&boss_->payload.state==0&&boss_->payload.at(0x50)!=4){auto&e=boss_->payload;if(game::intersects({14,12,106,70},ex.x,ex.y,{e.at(0x20),e.at(0x24),e.at(0x28),e.at(0x2c)},e.at(4),e.at(8))){sound(boss_->hurt());award_hit();}}
   if(later_actors_)for(auto*e:later_actors_->targets())if(!game::Stage2Actors::dying(*e)&&game::intersects({14,12,106,70},ex.x,ex.y,game::Stage2Actors::rectangle(*e),e->at(4),e->at(8))&&later_actors_->hurt(*e))award_hit();
   visit_later_weapon_targets([&](game::EntityPayload&e,const game::CollisionRect&r,auto hurt){if(game::intersects({14,12,106,70},ex.x,ex.y,r,e.at(4),e.at(8))&&hurt())award_hit();});
   ++ex.age;
  }
  explosions_.erase(std::remove_if(explosions_.begin(),explosions_.end(),[](const Explosion&e){return e.age>10;}),explosions_.end());
  target.flush_player();
  std::vector<CompanionHud::Target> companion_targets;
  for(auto*enemy:enemies_->targets())if(enemy->state!=(enemy->map_type==0?3:4))companion_targets.push_back({enemy->x,enemy->y,{28,14,46,71},[&,enemy]{enemies_->defeat(*enemy);award_hit();}});
  if(boss_&&boss_->payload.state==0&&boss_->payload.at(0x50)!=4){auto&e=boss_->payload;companion_targets.push_back({e.at(4),e.at(8),{e.at(0x20),e.at(0x24),e.at(0x28),e.at(0x2c)},[&]{sound(boss_->hurt());award_hit();}});}
  if(later_actors_)for(auto*e:later_actors_->targets())if(!game::Stage2Actors::dying(*e))companion_targets.push_back({e->at(4),e->at(8),game::Stage2Actors::rectangle(*e),[&,e]{if(later_actors_->hurt(*e))award_hit();}});
  // Shiro must use the same later-stage target registry as the other weapons.
  visit_later_weapon_targets([&](game::EntityPayload&e,const game::CollisionRect&r,auto hurt){
   companion_targets.push_back({e.at(4),e.at(8),r,[this,hurt]{if(hurt())award_hit();}});
  });
  companion_.tick(player,camera_x(),camera_y(),assets_,target,companion_targets);
  if(abilities_){abilities_->tick(camera_x(),camera_y(),target);for(auto&file:abilities_->sounds)if(play_sound_file)play_sound_file(file);abilities_->sounds.clear();}
  // Weapon collisions happen after the normal actor sound drain. Submit these
  // hit sounds now, before a scene/ending transition can skip the next update.
  for(const auto&file:enemies_->sound_requests)if(play_sound_file)play_sound_file(file);enemies_->sound_requests.clear();
  if(later_actors_){for(const auto&file:later_actors_->sounds)if(play_sound_file)play_sound_file(file);later_actors_->sounds.clear();}
  globals_[0x46cda8]=(companion_.active()||(abilities_&&abilities_->active()))?1:0;
  draw_pickup_hud(assets_,player,target);
  for(auto it=shots_.begin();it!=shots_.end();){if((*it)->owner_state==2){destroy_projectile(**it,0,*this);it=shots_.erase(it);}else ++it;}
 }
};
}



