
#Event Invite "#1# invites you to join a #2#.#*#"
#Event Slain "You have been slain by#*#"
#Event Slain "Returning to Bind Location#*#"
#Event Slain "You died."
#Event InvalidZone "#*#Invalid Zone#*#You will be redirected#*#"
#Event ConsentDelay "You must wait#*#between consents#*#"
#Event DelayOnZone "You have entered#*#"
#Event DelayOnZone "LOADING, PLEASE WAIT#*#"
#Event SummonFW "You are low on #1#."
#Event SummonFW "You are out of #1#."
#Event WornOff "Your #1# spell has worn off of #2#."
#Event Camping "It will take about#*#more seconds to prepare your camp#*#"
#Event GoM "You have been granted#*#gift of #1# to #2#!"
#Event OutOfStuff "Sorry, but you don't have everything you need for this recipe in your general inventory."
#Event Broke "#1# tells you#*#you can#*#t afford#*#"
#Event EQBC "<#1#> #2#"
#Event EQBC "[#1#(msg)] #2#"
#Event EQBC "[MQ2] mb- #2#"
#Event EQBC "mb- #2#"
#Event AAEarned "You have gained#*#ability point#*#"
#Event AACapped "You have reached the AA point cap#*#"
#Event OUTDOORS "You can only cast this spell in the outdoors#*#"
#Event OUTDOORS "You can't levitate in this zone#*#"
#Event OUTDOORS "You have entered an area where levitation effects do not function#*#"
|#event ResetMelee "You cannot see your target."
|#Event OoR "#*#no room left#*#"
#Event FeignD "You are flung into the air by#*#"
#Event Skillup "You have become better at #1#! #2#"

Sub Startup
   /if (!${Plugin[MQ2Eqbc].Name.Length}) {
      /squelch /plugin MQ2Eqbc noauto
      /if (!${Plugin[MQ2Eqbc].Name.Length}) {
         /echo ${If[${TimeStampOnEcho},[${Time}] ,]}You must have MQ2EQBC plugin to use this macro!!
         /endmacro
      }
   }
   /if (!${Plugin[MQ2Netbots].Name.Length}) {
      /squelch /plugin MQ2Netbots noauto
      /if (!${Plugin[MQ2Netbots].Name.Length}) {
         /echo ${If[${TimeStampOnEcho},[${Time}] ,]}You must have MQ2Netbots plugin to use this macro!!
         /endmacro
      }
   }
   /if (!${Plugin[MQ2Melee].Name.Length}) {
      /squelch /plugin MQ2Melee noauto
      /if (!${Plugin[MQ2Melee].Name.Length}) {
         /echo ${If[${TimeStampOnEcho},[${Time}] ,]}You must have MQ2Melee plugin to use this macro!!
         /endmacro
      }
      /bc Msg: You should configure MQ2Melee if you want to melee
   }
   /if (!${Plugin[MQ2Cast].Name.Length}) {
      /squelch /plugin MQ2Cast noauto
      /if (!${Plugin[MQ2Cast].Name.Length}) {
         /echo ${If[${TimeStampOnEcho},[${Time}] ,]}You must have MQ2Cast plugin to use this macro!!
         /endmacro
      }
   }
   /if (!${Plugin[MQ2Exchange].Name.Length}) {
      /squelch /plugin MQ2Exchange noauto
      /if (!${Plugin[MQ2Exchange].Name.Length}) {
         /echo ${If[${TimeStampOnEcho},[${Time}] ,]}You must have MQ2Exchange plugin to use this macro!!
         /endmacro
      }
   }
   /if (!${Plugin[MQ2AdvPath].Name.Length}) {
      /squelch /plugin MQ2AdvPath noauto
      /if (!${Plugin[MQ2AdvPath].Name.Length}) {
         /echo ${If[${TimeStampOnEcho},[${Time}] ,]}You must have MQ2AdvPath plugin to use this macro!!
         /endmacro
      }
   }
   /if (!${Plugin[MQ2MoveUtils].Name.Length}) {
      /squelch /plugin MQ2MoveUtils noauto
      /if (!${Plugin[MQ2MoveUtils].Name.Length}) {
         /echo ${If[${TimeStampOnEcho},[${Time}] ,]}You must have MQ2MoveUtils plugin to use this macro!!
         /endmacro
      }
   }
   /if (${Me.Class.ShortName.Equal[BRD]}) {
      /if (!${Plugin[MQ2Twist].Name.Length} ) {
         /squelch /plugin MQ2Twist noauto
         /if (!${Plugin[MQ2Twist].Name.Length} ) {
            /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Bards must have MQ2Twist plugin to use this macro!!
            /endmacro
         }
      }
   }

   /if (!${NetBots.Enable} || !${NetBots.Listen} || !${NetBots.Output}) {
      /netbots on send=on grab=on
   }
   /squelch /stick off

| EQBC/Chat related items
   /declare CurrentEQBCCommand string outer
   /declare SenderName string outer
| Mobs in current zone that are excluded from Debuffs/Detection (MBCommon.ini ExcludeList) - Added to AlertList
   /declare ExcludeList string outer
   /declare ExcludeListID string outer
   /declare AlertList int outer 20
| Spells not allowed in current zone (MBCommon.ini RestrictedList) - Usually Levitation-related
   /declare SpellsRestrictedByZone string outer
| Mobs in current zone that are Immune to certain spells (MBCommon.ini ImmuneList) - Usually Mez/Stun-related
   /declare ZoneSpellImmunes string outer
| Tank/Puller related items
   /declare TankName string outer
   /declare PullerName string outer
   /declare ManualTank bool outer FALSE
   /declare ManualPuller bool outer FALSE
   /declare TankID int outer 0
| Timers for 'extra' activities
   /declare MiscCheckTimer timer outer
   /declare SitCheckTimer timer outer
   /declare AACheckTimer timer outer
   /declare NeedFoodTimer timer outer
   /declare NeedDrinkTimer timer outer
| MakeCamp Items
   /declare CampStatus int outer
   /declare MakeCampX int outer
   /declare MakeCampY int outer
   /declare MakeCampD int outer
| Items related to Following
| Renamed FollowFlag to FollowState so MQ2Cast no longer sees it and thinks we are using advpath.inc
   /declare FollowState int outer
   /declare FollowID int outer
   /declare FollowName string outer
|  /declare FollowStateZoneDelay int outer 20
   /declare FollowDistanceCheck int outer 35
| Bard-Related Items. Bards use MQ2Twist (and do NOT use MQ2Cast)
   /declare BardCombatGems string outer
   /declare BardRestGems string outer
   /declare NextBardSlot int outer 40
   /declare CastResult string outer UNKNOWN
| All Player Classes
   /declare pcClasses string outer brd bst ber clr dru enc mag mnk nec pal rng rog shd shm war wiz
   /declare pcClassCount int outer 16
| Classes that have Pet Affinity (for Group Buffs)
   /declare petClasses string outer bst,enc,mag,nec,shd,shm
| Starting text of some standard chat messages we can immediately ignore so we don't waste time with them
   /declare IgnorePrefixes string outer Cast:,Buffed:,Debuffed:,Healing:,Meleeing:,Curing:,Selling:,Setting,attacking,Msg:,Evented:
| Simple On/Off Toggle Commands - Upper/Lower Case is important here. It has to match the variable declaration
   /declare OnOffCommands string outer DoAfk,DoBuffBot,DoBuffs,DoCombines,DoCures,DoDebuffs,DoEvents,DoForage,DoFW,DoHeals,DoLoot,DoMelee,DoMount,DoNamed,DoPet,DoPetShrink,DoPetToys,DoRanged,DoRez,DoSelfShrink,DoSit,DoTells,EchoScripts,IgnorePets
| Action Commands - Case is unimportant. Will call a Subroutine named "Command_" + uppercase version of it.   
   /declare ActionCommands string outer Abort,AddBuff,AddDebuff,AddTank,Assist,Attack,AutoBank,AutoSkills,Backoff,Buffup,Buy,Campout,CheckInv,DoAA,DoCommand,DoList,DoPull,DoQuest,DropPet,DropTank,Duck,Exclude,ExcludeID,Follow,GetBank,Give,Goto,GroupUp,HailDelay,HailTarget,HandIn,Help,Holdup,Include,IncludeID,InviteAll,LetsRoll,ListTanks,LTeach,MakeCamp,Mana,MBPause,MBWayPlay,MBWayRec,OffTank,PetBack,PetToys,Port,Recast,SaveGroup,SayDelay,SayTarget,Script,Search,Sell,SetINIvar,SetLoot,SetPuller,SetTank,SetTanks,SingleFile,Stop,SwapItem,SwapSave,Switch,TankAndHeals,XBots,
   /declare CommandText string outer
| Command-Toggled Statuses that are not ini entries
   /declare MBPause bool outer
   /declare DoBuffBot bool outer
   /declare DoCombines bool outer FALSE
   /declare DoQuest bool outer TRUE
   /declare EchoScripts bool outer FALSE
| PetToys items
   /declare PTCount int outer 0
   /declare PTSpell[5] string outer
   /declare PTItem1[5] string outer
   /declare PTItem2[5] string outer
| MQ2Cast items
   /declare castReturn string outer NULL
   /declare noFeigning bool outer FALSE
   /declare noInvis bool outer FALSE
   /declare interruptFlag bool outer FALSE
| Equipment Slots (23) - Order is Important
   /declare slotNames string outer charm,leftear,head,face,rightear,neck,shoulder,arms,back,leftwrist,rightwrist,ranged,hands,mainhand,offhand,leftfinger,rightfinger,chest,legs,feet,waist,powersource,ammo
   /declare numSlots int outer ${Math.Calc[${slotNames.Count[,]}+1]}
   /declare equippedArray[${numSlots}] string outer
| Used in Give routines
   /declare gX int outer 0
   /declare gY int outer 0
   /declare gCount int outer 0
| Items for handling WornOff Event
   /declare QueueCount int outer
   /declare DoBuffQueue[25,2] int outer
| Additional Event-related items
   /declare OutDoorsFail bool outer FALSE
   /declare ConsentDelayDetected bool outer FALSE
| Items used to try and deal with the EQ Stuck Gems issue (not sure if this is still happening or not)
   /declare GemStuckTimer timer outer
   /declare GemStuckID int outer
   /declare GemStuckRetry timer outer
   /declare SpellWasCast bool outer FALSE
 
   /declare ZoneTypes string outer Indoor Dungeon,Outdoor,Outdoor City,Dungeon City,Indoor City,Outdoor Dungeon    
   /declare AllNonCombatZones string outer GuildHall,GuildLobby,PoKnowledge,Nexus,Bazaar,AbysmalSea,potranquility,abysmal
   
   /declare CurMerchant int outer 0
   /declare MBCurrentSpell string outer
   /declare NamedID int outer
   /declare ZoneName string outer
   /declare PSTargetID int outer 0
   /declare ValidSpellGems string outer
   /declare bInvalidZone bool outer FALSE
   
   /declare a int local 0 

| Vars from MBCommon
   /call LoadINIVar Settings BeepCommand "beep ding.wav" BeepCommand "${MBCommon}" string
   /call LoadINIVar Settings CorpseConsent "Group,Raid,Guild,NetBots" CorpseConsent "${MBCommon}" string
   /call LoadINIVar Settings DefaultJoinChannels FALSE DefaultJoinChannels "${MBCommon}" bool
   /call LoadINIVar Settings DefaultNewFormat 1 DefaultNewFormat "${MBCommon}" string   
   /call LoadINIVar Settings IgnoreBuffLevelRestrictions FALSE IgnoreBuffLevelRestrictions "${MBCommon}" bool
   /call LoadINIVar Settings NonCombatZones " " NonCombatZones "${MBCommon}" string
   /call LoadINIVar Settings ReloadGemsOnDeath ${If[${MacroQuest.BuildName.Equal[emu]},TRUE,FALSE]} ReloadGemsOnDeath "${MBCommon}" bool

| Vars from Char Ini
   /call LoadINIVar Settings AAAnnounce " " AAAnnounce "${IniFile}" string
   /call LoadINIVar Settings AABank 0 AABank "${IniFile}" int
   /call LoadINIVar Settings AAtoNormal FALSE AAtoNormal "${IniFile}" bool
   /call LoadINIVar Settings AAVerbose FALSE AAVerbose "${IniFile}" bool
   /call LoadINIVar Settings AAWarning ${If[${MacroQuest.BuildName.Equal[emu]},0,90]} AAWarning "${IniFile}" int
   /call LoadINIVar Settings AfkMessage " " AFKMessage "${IniFile}" string
   /call LoadINIVar Settings AllowBuffRequests " " AllowBuffRequests "${IniFile}" string
   /call LoadINIVar Settings AllowMGB 0 AllowMGB "${IniFile}" int
   /call LoadINIVar Settings DeathSlot "FALSE" DeathSlot "${IniFile}" string
   /call LoadINIVar Settings DoAA FALSE DoAA "${IniFile}" bool
   /call LoadINIVar Settings DoAfk FALSE DoAfk "${IniFile}" bool
   /call LoadINIVar Settings DoBuffs FALSE DoBuffs "${IniFile}" bool
   /call LoadINIVar Settings DoCures FALSE DoCures "${IniFile}" bool
   /call LoadINIVar Settings DoDebuffs FALSE DoDebuffs "${IniFile}" bool
   /call LoadINIVar Settings DoEvents FALSE DoEvents "${IniFile}" bool
   /call LoadINIVar Settings DoForage FALSE DoForage "${IniFile}" bool
   /call LoadINIVar Settings DoFW FALSE DoFW "${IniFile}" bool
   /call LoadINIVar Settings DoHeals FALSE DoHeals "${IniFile}" bool
   /call LoadINIVar Settings DoLoot FALSE DoLoot "${IniFile}" bool
   /call LoadINIVar Settings DoMelee FALSE DoMelee "${IniFile}" bool
   /call LoadINIVar Settings DoMount FALSE DoMount "${IniFile}" bool   
   /call LoadINIVar Settings DoNamed TRUE DoNamed "${IniFile}" bool
   /call LoadINIVar Settings DoPet FALSE DoPet "${IniFile}" bool
   /call LoadINIVar Settings DoPetShrink TRUE DoPetShrink "${IniFile}" bool
   /call LoadINIVar Settings DoPull FALSE DoPull "${IniFile}" bool   
   /call LoadINIVar Settings DoRez TRUE DoRez "${IniFile}" bool
   /call LoadINIVar Settings DoSelfShrink FALSE DoSelfShrink "${IniFile}" bool
   /call LoadINIVar Settings DoSit FALSE DoSit "${IniFile}" bool
   /call LoadINIVar Settings DoStuckGems FALSE DoStuckGems "${IniFile}" bool
   /call LoadINIVar Settings DoTells TRUE DoTells "${IniFile}" bool
   /call LoadINIVar Settings EchoSkillups FALSE EchoSkillups "${IniFile}" bool
   /call LoadINIVar Settings FollowDistance 20 FollowDistance "${IniFile}" string
|  /call LoadINIVar Settings ForageIni "forage.ini" ForageIni "${IniFile}" string   
   /call LoadINIVar Settings GoMNuke " " GoMNuke "${IniFile}" string
   /call LoadINIVar Settings IgnorePets TRUE IgnorePets "${IniFile}" bool
   /call LoadINIVar Settings InfoMsgLevel 0 InfoMsgLevel "${IniFile}" int
   /call LoadINIVar Settings JoinMBChannels "${DefaultJoinChannels}" JoinMBChannels "${IniFile}" bool
   /call LoadINIVar Settings MasterList "Netbots" MasterList "${IniFile}" string
   /call LoadINIVar Settings NetworkINI " " NetworkINI "${MBCommon}" string
   /call LoadINIVar Settings PetAssist 0 PetAssist "${IniFile}" int
   /call LoadINIVar Settings PullerName " " AllPullerName "${IniFile}" string
   /call LoadINIVar Settings Radius 100 Radius "${IniFile}" int
   /call LoadINIVar Settings ReturnToLoc FALSE ReturnToLoc "${IniFile}" bool
   /call LoadINIVar Settings RezAcceptCall FALSE RezAcceptCall "${IniFile}" bool
   /call LoadINIVar Settings RezGiveUpSeconds 6000 RezGiveUpSeconds "${IniFile}" int
   /call LoadINIVar Settings RezMinPct 96 RezMinPct "${IniFile}" int
   /call LoadINIVar Settings SitAggroRadiusCheck 30 SitAggroRadiusCheck "${IniFile}" int
   /call LoadINIVar Settings SitEndurPct ${If[${Select[${Me.Class.ShortName},CLR,DRU,SHM,ENC,NEC,MAG,WIZ,BRD]},0,95]} SitEndurPct "${IniFile}" int   
   /call LoadINIVar Settings SitManaPct ${If[${Me.MaxMana}==0,0,95]} SitManaPct "${IniFile}" int
   /call LoadINIVar Settings SPetAssist 1 SPetAssist "${IniFile}" int
   /call LoadINIVar Settings StuckGemsAA " " StuckGemsAA "${IniFile}" string
   /call LoadINIVar Settings SummonFood " " SumFood "${IniFile}" string
   /call LoadINIVar Settings SummonDrink " " SumDrink "${IniFile}" string   
   /call LoadINIVar Settings TankName " " AllTankName "${IniFile}" string
   /call LoadINIVar Settings TraderName " " TraderName "${IniFile}" string
   /call LoadINIVar Settings UseHeadings FALSE UseHeadings "${IniFile}" bool
   /call LoadINIVar Settings ZRadius 50 ZRadius "${IniFile}" int
   /if (${Me.Class.ShortName.Equal[MAG]}) {
      /call LoadINIVar Settings DoPetToys FALSE DoPetToys "${IniFile}" bool
      /call LoadINIVar Settings PetToys1 " " PetToys1 "${IniFile}" string
      /call LoadINIVar Settings PetToys2 " " PetToys2 "${IniFile}" string
      /call LoadINIVar Settings PetToys3 " " PetToys3 "${IniFile}" string
      /call LoadINIVar Settings PetToys4 " " PetToys4 "${IniFile}" string
      /call LoadINIVar Settings PetToys5 " " PetToys5 "${IniFile}" string
      /for a 1 to 5
         /if (${PetToys${a}.Length}) {
            /varcalc PTCount ${PTCount}+1
            /varset PTSpell[${PTCount}] ${PetToys${a}.Arg[1,,]}
            /varset PTItem1[${PTCount}] ${If[${PetToys${a}.Arg[2,,].NotEqual[NULL]},${PetToys${a}.Arg[2,,]},]}
            /varset PTItem2[${PTCount}] ${If[${PetToys${a}.Arg[3,,].NotEqual[NULL]},${PetToys${a}.Arg[3,,]},]}
         }
      /next a
   }

   /if (${NonCombatZones.Length}) {
      /for a 1 to ${Math.Calc[${NonCombatZones.Count[,]}+1]}
         /if (${NonCombatZones.Arg[${a},,].Length}>2) {
            /varset AllNonCombatZones ${AllNonCombatZones},${NonCombatZones.Arg[${a},,]}
         }
      /next a
   }

   /if (${AllPullerName.Left[6].Equal[manual]}) {
      /varset ManualPuller TRUE
      /varset AllPullerName ${AllPullerName.Right[-7]}
   }
   /call SetPuller

   /if (${AllTankName.Left[6].Equal[manual]}) {
      /varset ManualTank TRUE
      /varset AllTankName ${AllTankName.Right[-7]}
   }
   /call SetTank

   /if (!${Select[${DeathSlot},TRUE,FALSE]}) {
      /ini "${IniFile}" "Settings" "DeathSlot" "FALSE"
   }

| Adjustments for Casters / Non-Casters
   /if (${Me.MaxMana}) {
      /for a 1 to ${Me.NumGems}
         /varset ValidSpellGems ${If[!${ValidSpellGems.Length},,${ValidSpellGems},]}${a}
      /next a
      /if (${MacroQuest.BuildName.Equal[emu]}) {
         /varset DoStuckGems FALSE
| Save Spell Set for reload after a death.
         /sss ModBot2
      } else {
         /varset ReloadGemsOnDeath FALSE
      }
   } else {
      /varset ValidSpellGems 0   
      /varset DoStuckGems FALSE
      /varset ReloadGemsOnDeath FALSE
   }

   /if (${Me.Class.ShortName.Equal[BRD]}) {
      /squelch /twist clear
      /squelch /twist reset
      /squelch /twist reload
   }   
   
   /squelch /makecamp leash 200
   /squelch /makecamp radius 10
   /if (!${MakeCamp.Leash}) /squelch /makecamp leash
   /squelch /makecamp off

   /call LoadINIVar Settings LoadAlias 1 LoadAlias "${MBCommon}" int
   /if (${LoadAlias}) {
      /alias /mb /echo MB-
      /ini "${MBCommon}" "Settings" "LoadAlias" 0
   }

   /call ProcessExcludeList zone
   /varset ZoneName ${Zone.Name}

   /doevents flush
/return


Sub Event_EQBC(EQBCSay,EQBCSender,EQBCCommand)
   /if (!${Defined[EQBCSender]}) {
      /declare EQBCSender string local
   }
   /if (!${DoBuffBot} && !${Select[${EQBCSender},${MasterList}]} && ${EQBCSender.Length} && !${MasterList.Equal[Netbots]} && ${NetBots.Client.Find[${EQBCSender}]}) /return
   /varset EQBCCommand ${EQBCSay.Right[-${Math.Calc[${EQBCSay.Find[${EQBCCommand}]}-1]}]}
   /if (${Select[${EQBCSender},${MasterList}]} || (${EQBCSender.Length} && (${MasterList.Equal[Netbots]} && ${NetBots.Client.Find[${EQBCSender}]}) || ${EQBCSender.Equal[${Me.Name}]})) {
      /if (${Select[${EQBCCommand.Arg[1]},${IgnorePrefixes}]}) /return
      /if (${Select[${EQBCCommand.Arg[2]},${IgnorePrefixes}]}) /return
      /if (${EQBCCommand.Find[immune to]}) /return
      /if (${EQBCCommand.Left[9].Equal[Tell from]}) /return
   }
   /if (!${EQBCSender.Length}) /varset EQBCSender LOCAL
   /if (${EQBCCommand.Arg[1].Equal[${Me.Name}]}) {
      /varset EQBCCommand ${EQBCCommand.Mid[${Math.Calc[${Me.Name.Length}+1]},${Math.Calc[${EQBCCommand.Length}-${Me.Name.Length}]}]}
   }
   /varset CurrentEQBCCommand ${EQBCCommand}
   /call Event_chat "eqbc" "${EQBCSender}" "${EQBCCommand}"
/return


Sub Event_Chat(MsgType,MsgFrom,MsgText)
|/echo EC ${MsgType} - ${MsgFrom}
   /doevents FeignD
   /if (${Select[${MsgText.Arg[1]},${IgnorePrefixes}]}) /return
   /if (${MsgText.Find[ master]} || ${MsgFrom.Equal[${Me.Pet.Name}]} || ${Spawn[npc ${MsgFrom}].ID}) /return
   /if (${MsgType.Equal[eqbc]}) /varset MsgText ${CurrentEQBCCommand}
   
   /declare a int local
   /if (${MsgType.NotEqual[EQBC]}) {
      /if (${MsgFrom.Left[1].Compare[ ]}<0) {
         /varset MsgFrom ${MsgFrom.Right[-2].Left[-1]}
      }
   }

   /if (${Select[${MsgFrom},${NetBots.Client},LOCAL,${Me.Name}]} && ${MsgText.Find[reconnect]}) {
      /if (${MsgText.Arg[2, ].Length}>0) {
         /if (${EQBC.Connected}) /bccmd quit
         /delay 5 !${EQBC.Connected}
         /delay 5
         /bccmd connect ${MsgText.Arg[2, ]} ${If[${MsgText.Arg[3, ].Length},${MsgText.Arg[3, ]},]}
      } else {
         /bccmd reconnect
      }
      /return
   }
   /if (!${DoBuffBot} && !${Select[${MsgFrom},${MasterList},LOCAL,${Me.Name}]} && (!${MasterList.Equal[Netbots]} || !${NetBots.Client.Find[${MsgFrom}]})) {
      /if (!${NetBots[${MsgFrom}].ID} && ${MsgType.Equal[TELL]} && ${DoTells}) {
         /if (!${MsgFrom.Equal[${Me.Name}]} && !${MsgFrom.Equal[${Me.Pet.CleanName}]} && !${Spawn[npc ${MsgFrom}].ID}) /call TellEvent ${MsgFrom} "${MsgText}"
      }
      /if (${AllowBuffRequests.Length}) {
         /if (${MsgType.NotEqual[eqbc]} && !${Select[${MsgFrom},${MasterList},LOCAL]} && (!${MasterList.Equal[Netbots]} || !${NetBots.Client.Find[${MsgFrom}]})) {
            /if (${AllowBuffRequests.Find[All]} || (${AllowBuffRequests.Find[Group]} && ${Group.Member[${MsgFrom}].Index}) || (${AllowBuffRequests.Find[Raid]} && ${Raid.Member[${MsgFrom}].Level}) || (${AllowBuffRequests.Find[Guild]} && ${Me.Guild.Length} && ${Me.Guild.Equal[${Spawn[${MsgFrom}].Guild}]}) || (${AllowBuffRequests.Find[Fellowship]} && ${Me.Fellowship.Member[${MsgFrom}].Level})) {
               /for a 1 to ${Math.Calc[${MsgText.Count[ ]}+1]}
                  /if (${ABAliasList.Find[${MsgText.Arg[${a}]}]}) /call CastCall ${MsgFrom} "cast ${MsgText.Arg[${a}]}"
               /next a
            }
         }
      }
      /return
   }
   /if (${MsgFrom.Equal[local]}) {
      /varset SenderName ${Me.Name}
   } else {
      /varset SenderName ${MsgFrom}
   }
| If BuffBot, add 'cast'
   /if (${DoBuffBot}) {
      /if (${MsgText.Arg[1].Equal[help]}) /return
      /varset MsgText cast ${MsgText}
   }
| Check for CastCall variations   
   /if (${Select[${MsgText.Arg[1]},cast,qcast,scast,mgb,bomb,tmgb,tbomb]} || (${Me.Class.ShortName.Equal[brd]} && ${Select[${MsgText.Arg[1]},sing,sung]})) {
      /if (${MsgText.Arg[2].Equal[help]}) {
         /call Help ${MsgText.Arg[1]}
      } else {
| Copy to CommandText to preserve any quotes (don't let the subroutine call parse the string)
         /varset CommandText ${MsgText}
         /call CastCall "${SenderName}"
         /return
      }
   }
| Handle MBP Abbreviation (Expand it)
   /if (${MsgText.Arg[1].Equal[MBP]}) {
      /varset MsgText MBPause${MsgText.Right[-3]}
   }
| Handle Port Evac Quickies
   /if (${Select[${MsgText.Arg[1]},EvacNow,Evac!,Evac!!]}) {
      /varset MsgText Port Evac
   }
   /if (${Select[${MsgText.Arg[1]},bcskill]}) {
      /bc My "${MsgText.Right[-8]}" is ${Me.Skill[${MsgText.Right[-8]}]}
      /return 
   }
| Check our Command Lists
   /if (${Select[${MsgText.Arg[1]},${ActionCommands}]}) {
      /if (${MsgText.Arg[2].Equal[help]}) {
         /call Help ${MsgText.Arg[1]}
      } else {
| Copy to CommandText to preserve any quotes (don't let the subroutine call parse the string)
         /varset CommandText ${MsgText}
         /call Command_${ActionCommands.Arg[${Select[${MsgText.Arg[1]},${ActionCommands}]},,].Upper}
      }
   } else /if (${Select[${MsgText.Arg[1]},${OnOffCommands}]}) {
      /if (${MsgText.Arg[2].Equal[help]}) {
         /call Help ${MsgText.Arg[1]}
      } else {   
         /call TFToggle ${OnOffCommands.Arg[${Select[${MsgText.Arg[1]},${OnOffCommands}]},,]} ${MsgText.Arg[2]}
      }
| Handle our legacy DoSongs alias
   } else /if (${MsgText.Arg[1].Equal[DoSongs]}) {
      /if (${MsgText.Arg[2].Equal[help]}) {
         /call Help ${MsgText.Arg[1]}
      } else {
         /call TFToggle DoBuffs ${MsgText.Arg[2]}
      }
   }
| Anything else, we just ignore as chatter
/return


Sub LoadINIVar(IniSection,IniVar,IniValue,MacroVar,MyIni,VarType)
|/echo -${IniSection}- -${IniVar}- -${IniValue}- -${MacroVar}- -${MyIni}- -${VarType}-
   /declare NoVar bool local ${If[${MacroVar.Equal[NOVAR]},true,false]}
   /declare IniString string local ${Ini["${MyIni}",${IniSection},${IniVar},NOTFOUND]}
   /if (!${NoVar}) {
      /if (!${Defined[${MacroVar}]} && ${VarType.Length}) {
         /declare ${MacroVar} ${VarType} outer
      }
      /varset ${MacroVar} ${IniString}
   }
   /if (${IniString.Equal["NOTFOUND"]}) {
      /if (${NoVar}) {
         /ini "${MyIni}" "${IniSection}" "${IniVar}" " "
      } else {
         /varset ${MacroVar} ${IniValue}
         /ini "${MyIni}" "${IniSection}" "${IniVar}" "${${MacroVar}}"
      }
   }
/return


Sub CharmBroke
   /if (${Me.Pet.ID}) /return
   /bc Msg: LOST PET - LOST PET
   /if (!${ADCharmAD} || !${DoPet}) /return
   /if (${PetAssist}) /varset PetAssist 1
   /if (${SPetAssist}) /varset SPetAssist 1
   /if (!${Me.Mount.ID} && ${Me.Sitting}) /sit off
   /varset ADCheckTimer 0
   /if (${DoDebuffs} && ${ADCount}) /call ADDebuffCheck
/return


Sub AFK
   /docommand ${If[(${AFKMessage.Length} && !${AFKMessage.Equal[AFK Message]}),/afk ${AFKMessage},/afk]}
/return


Sub Event_Broke(Line,MName)
   /if (!${Merchant.Open}) /return
   /if (${Merchant.CleanName.NotEqual[${MName}]}) /return
   /echo ${If[${TimeStampOnEcho},[${Time}] ,]}-->You do not have enough money for that!
   /docommand /${BeepCommand}
/return TRUE


Sub Event_SummonFW(Line,ExtraStuff)
   /if (!${DoFW}) /return
   /if (${ExtraStuff.Find[food]}) /varset NeedFoodTimer 3m
   /if (${ExtraStuff.Find[drink]}) /varset NeedDrinkTimer 3m
/return
   

Sub SummonFW(string FWType)
   /if (${Me.Invis}) {
      /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Cannot Summon ${FWType} while Invis
   } else /if (${ADMobCount}) {
      /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Cannot Summon ${FWType} while mobs are present
   } else {
      /if (${Me.Casting.ID} && !${Me.Class.ShortName.Equal[brd]}) {
         /delay 5s !${Me.Casting.ID}
      }
      /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Summoning ${FWType}
      /call MQ2Cast "${Sum${FWType}.Arg[1,${PipeChar}]}" ${Sum${FWType}.Arg[2,${PipeChar}]} 10s CastCheck
      /delay 10s ${Cursor.ID}
      /call ClearCursor force
      /varset Need${FWType}Timer
   }
/return


Sub Event_Invite(string eLine,string ePerson,string eType)
   /if (!${Select[${eType},group,raid]}) /return
   /if (${Select[${ePerson},${MasterList}]} || ${NetBots[${ePerson}].ID} || ${Spawn[guild ${ePerson}].ID}) {
      /if (!${Me.Class.ShortName.Equal[brd]} && ${Me.Casting.ID}) {
         /delay 10s !${Me.Casting.ID}
      }
      /delay 1
      /invite
      /delay 1s
      /if (${Window[ConfirmationDialogBox].Open}) {
         /nomodkey /notify ConfirmationDialogBox CD_Yes_Button leftmouseup
      }
   }
/return


Sub Event_DelayOnZone(ZoneText)
   /if (${ZoneText.Find[Drunken Monkey]} || ${ZoneText.Find[effects]}) /return
   /delay 600s ${Me.ID}
   /delay 5s
   /doevents flush delayonzone
| Check if we died. If we died, the slain code will also check if zoned and handle it.   
   /doevents slain
   /call DoZoning
/return


Sub DoZoning(string ImSlain)
   /if (${CZoneID}==${Zone.ID}) /return
   /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Zoning Detected...
   /varset CZoneID ${Zone.ID}
   /if (${ImSlain.NotEqual[slain]}) {
      /varset DoDebuffs FALSE
      /varset DoMelee FALSE
      /varset DoPull FALSE
      /call ResetFunctions
      /if (${FollowState}) {
         /varset FollowState 10
      }
   }
   /if (${DoAfk} && !${Me.AFK}) /call AFK
   /call ProcessExcludeList zone
   /varset ZoneName ${Zone.Name}
   /call LoadSpellImmunes
   /call LoadRestrictedSpells
   /if (${Me.Class.ShortName.Equal[brd]}) {
      /call ResetBardGems
   }
   /call LootCleanup zone
   /if (${ImSlain.NotEqual[slain]} || ${MacroQuest.BuildName.NotEqual[emu]}) {
      /call Command_SWAPITEM "swapitem zone"
   }
   /doevents flush
   /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Zoning Complete.
   /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Welcome to ${Zone.Name} (${ZoneTypes.Arg[${Math.Calc[${Zone.Type}+1]},,]})
/return


Sub Event_Slain
   /docommand /${BeepCommand}
   /bc Msg: DEATH DETECTED - Putting all functions on hold.
   /echo ${If[${TimeStampOnEcho},[${Time}] ,]}DEATH DETECTED - Putting all functions on hold.
   /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Setting - DoBuffs,DoCures,DoDebuffs,DoEvents,DoHeals,DoMelee,DoPull FALSE   
   /echo ${If[${TimeStampOnEcho},[${Time}] ,]}You must issue the appropriate command to restart each function (or restart the macro).
   /delay 600s ${Me.ID}
   /delay 5s
   /doevents flush slain
   /varset DoBuffs FALSE
   /varset DoCures FALSE
   /varset DoDebuffs FALSE
   /varset DoEvents FALSE
   /varset DoHeals FALSE
   /varset DoMelee FALSE
   /varset DoPull FALSE
   /call ResetFunctions
   /call AdvPlugCall off
   /varset bInvalidZone FALSE
   /doevents InvalidZone
   /if (${bInvalidZone}) {
      /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Invalid Zone. Waiting 15s for redirect and zone.
	  /delay 15s
      /delay 600s ${Me.ID}
   }
   /delay 2s
   /nomodkey /keypress left
   /nomodkey /keypress right
   /call DoZoning slain
| Re-load Spell Gems (EMU)
   /if (${ReloadGemsOnDeath}) {
      /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Reloading Spell Gems.
      /ssm ModBot2
   }
| Consent, if needed.
   /call DoCorpseConsent
   /if (${ReloadGemsOnDeath} && ${Cast.Status.Find[M]}) {
      /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Continuing to load Spell Gems...
      /delay 30s !${Cast.Status.Find[M]}
   }
   /call Wait4Rez
   /delay 3s
   /delay 20s ${Cast.Status.Equal[i]}
   /doevents delayonzone
   /doevents flush
/return


Sub Wait4Rez
   /declare rezPct int local
   /declare callFlag bool local
   /declare stateFlag bool local 
   /declare RezWaitTimer timer ${RezGiveUpSeconds}s
   /if (!${DoRez}) {
      /bc Msg: DoRez is FALSE. Taking no Rez actions.
      /varset stateFlag TRUE
   } else /if (${RezMinPct}>100 && !${RezAcceptCall}) {
      /bc Msg: Rez settings do not allow for auto-accepting of rez/call. Taking no Rez actions.
      /varset stateFlag TRUE
   } else {
      /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Awaiting rez
      /varset stateFlag FALSE
   }
   /if (${stateFlag}) {
      /if (!${Select[${Me.State},HOVER,DEAD]}) /return
      /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Waiting for respawn from Hover/Dead
   } else {
      /if (${RezMinPct}<101) /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Auto-accepting rez of ${RezMinPct}% and above
      /if (${RezAcceptCall}) /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Auto-accepting Call to Corpse for rez later
   }
   /delay 2s
:checkforrez
| Not rezzing, but wait until we respawn
   /if (${stateFlag}) {
      /if (!${Select[${Me.State},HOVER,DEAD]}) /return
      /delay 3s
      /goto :checkforrez
   }
   /if (!${Window[ConfirmationDialogBox].Open} && !${RezWaitTimer}) {
      /bc Msg: Rez timer expired. Camping to give time to rez me later.
      /ini "${IniFile}" "Settings" "DeathSlot" "TRUE"
      /camp desk
      /delay 5s
      /endmacro
   }
   /delay 10s ${Window[ConfirmationDialogBox].Open} 
   /if (!${Window[ConfirmationDialogBox].Open}) /goto :checkforrez
   /delay 1s
   /if (${Bool[${Window[ConfirmationDialogBox].Child[CD_TextOutput].Text.Find["(0 percent)"]}]}) {
       /varset rezPct 0
       /varset callFlag FALSE
   } else /if (${Bool[${Window[ConfirmationDialogBox].Child[CD_TextOutput].Text.Find["(10 percent)"]}]}) { 
       /varset rezPct 10
       /varset callFlag FALSE
   } else /if (${Bool[${Window[ConfirmationDialogBox].Child[CD_TextOutput].Text.Find["(20 percent)"]}]}) { 
       /varset rezPct 20
       /varset callFlag FALSE
   } else /if (${Bool[${Window[ConfirmationDialogBox].Child[CD_TextOutput].Text.Find["(35 percent)"]}]}) { 
       /varset rezPct 35
       /varset callFlag FALSE
   } else /if (${Bool[${Window[ConfirmationDialogBox].Child[CD_TextOutput].Text.Find["(50 percent)"]}]}) { 
       /varset rezPct 50
       /varset callFlag FALSE
   } else /if (${Bool[${Window[ConfirmationDialogBox].Child[CD_TextOutput].Text.Find["(60 percent)"]}]}) { 
       /varset rezPct 60
       /varset callFlag FALSE
   } else /if (${Bool[${Window[ConfirmationDialogBox].Child[CD_TextOutput].Text.Find["(75 percent)"]}]}) { 
       /varset rezPct 75
       /varset callFlag FALSE
   } else /if (${Bool[${Window[ConfirmationDialogBox].Child[CD_TextOutput].Text.Find["(90 percent)"]}]}) { 
       /varset rezPct 90
       /varset callFlag FALSE
   } else /if (${Bool[${Window[ConfirmationDialogBox].Child[CD_TextOutput].Text.Find["(96 percent)"]}]}) { 
       /varset rezPct 96
       /varset callFlag FALSE
   } else /if (${Bool[${Window[ConfirmationDialogBox].Child[CD_TextOutput].Text.Find["(100 percent)"]}]}) {
       /varset rezPct 100
       /varset callFlag FALSE
   } else /if (${Bool[${Window[ConfirmationDialogBox].Child[CD_TextOutput].Text.Find["you will still be able to get a resurrection later"]}]}) {
       /varset rezPct 0
       /varset callFlag TRUE
   } else {
       /varset rezPct 0
       /varset callFlag FALSE
   }
   /if (${rezPct}>=${RezMinPct} || (${RezAcceptCall} && ${callFlag})) /goto :acceptrez
   /if (${callFlag}) {
      /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Declining call to corpse. Continuing to wait.
   } else {
      /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Declining ${rezPct}% rez. Continuing to wait.
   }
   /nomodkey /notify ConfirmationDialogBox No_Button leftmouseup
   /goto :checkforrez
:acceptrez
   /if (${callFlag}) {
      /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Accepting call to corpse.
   } else {
      /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Accepting ${rezPct}% rez.
   }
   /nomodkey /notify ConfirmationDialogBox Yes_Button leftmouseup 
   /delay 2s
| EMU doesn't use Respawn window   
   /if (${Window[RespawnWnd].Open}) {
      /nomodkey /notify RespawnWnd RW_OptionsList listselect 2  
      /delay 1s 
      /nomodkey /notify RespawnWnd RW_SelectButton leftmouseup 
      /delay 2s 
   }
   /ini "${IniFile}" "Settings" "DeathSlot" "FALSE"
   /delay 3s !${Window[RespawnWnd].Open}
   /delay 600s ${Me.ID}
   /delay 2s
| Loot my corpse (EMU)
   /if (${MacroQuest.BuildName.Equal[emu]}) {
      /call LootMyCorpse
   }
   /delay 1s 
/return


Sub DoCorpseConsent
   /if (!${CorpseConsent.Length}) /return
   /declare x int local
   /declare GroupConsented bool local FALSE
   /declare RaidConsented bool local FALSE
   /declare GuildConsented bool local FALSE
   /if (${Me.GroupSize}) {
      /if (${Window[OptionsWindow].Child[OGP_AutoConsentGroupCheckBox].Checked}) {
         /varset GroupConsented TRUE
      } else /if (${CorpseConsent.Find[group]}) {
         /call DoConsent Group
         /varset GroupConsented TRUE
      }
   }
   /if (!${Select[${Window[RaidWindow].Child[RAID_PlayerCountLabel].Text},NULL,0]}) {
      /if (${Window[OptionsWindow].Child[OGP_AutoConsentRaidCheckBox].Checked}) {
         /varset RaidConsented TRUE
      } else /if (${CorpseConsent.Find[raid]}) {
         /call DoConsent Raid
         /varset RaidConsented TRUE
      }
   }
   /if (${Me.Guild.Length}) {
      /if (${Window[OptionsWindow].Child[OGP_AutoConsentGuildCheckBox].Checked}) {
         /varset GuildConsented TRUE
      } else /if (${CorpseConsent.Find[guild]}) {
         /call DoConsent Guild
         /varset GuildConsented TRUE
      }
   }
   /if (${CorpseConsent.Find[netbots]}) {
      /for x 1 to ${NetBots.Counts}
| Consent others we haven't done above. We're trying to limit it so we aren't stuck in this loop if we don't need to be
         /if (${NetBots.Client.Arg[${x}].Equal[${Me.CleanName}]}) /continue
         /if (${GroupConsented}) {
            /if (${Group.Member[${NetBots.Client.Arg[${x}]}].Index}) /continue
         }
         /if (${RaidConsented}) {
            /if (${Raid.Member[${NetBots.Client.Arg[${x}]}].Level}) /continue
         }
         /if (${GuildConsented}) {
            /if (${Window[GuildManagementWnd].Child[MemberList].List[${Window[GuildManagementWnd].Child[MemberList].List[=${NetBots.Client.Arg[${x}]}]},1].Equal[${NetBots.Client.Arg[${x}]}]}) /continue
         }
         /call DoConsent "${NetBots.Client.Arg[${x}]}"
      /next x
   }
/return


Sub DoConsent(string tConsent)
   /doevents flush ConsentDelay
   /varset ConsentDelayDetected FALSE
   /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Consenting - ${tConsent}
   /consent ${tConsent}
   /delay 1s
   /doevents ConsentDelay
| Consent. EQ client gives a message about waiting 2s between consents. But even a delay of 3s here was often not long enough.
   /if (${ConsentDelayDetected}) {
      /delay 3s
      /consent ${tConsent}
   }
/return


Sub Event_ConsentDelay
   /varset ConsentDelayDetected TRUE
/return


Sub LootMyCorpse
   /declare x int local
   /if (!${Spawn[${Me.CleanName}'s corpse].ID}) /return
   /target mycorpse
   /delay 1s ${Target.Type.Equal[Corpse]} && ${Target.Name.Find[${Me.CleanName}'s]}
   /corpse
   /delay 1s ${Target.Distance}<20
   /delay 5
   /loot
   /delay 1s ${Corpse.Open}
   /delay 5
| Give time for Loot Lag
   /while (1) {
      /varset x ${Corpse.Items}
      /delay 1s ${x}!=${Corpse.Items}
      /if (${x}==${Corpse.Items}) /break
      /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Loot lag...
   }
   /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Looting my corpse...
   /nomodkey /notify LootWnd LW_LootAllButton leftmouseup
   /delay 30s !${Corpse.Open}
/return


Sub ResetFunctions
| Common things to Clear/Reset on Death/Zone
   /squelch /stick off
   /makecamp off
   /varset CampStatus 0
   /if (${PetAssist}) /varset PetAssist 1
   /if (${SPetAssist}) /varset SPetAssist 1
   /if (${Melee.Combat}) /melee reset
   /varset SelfShrinkTries 0
   /varset ACStartY ${Me.Y}
   /varset ACStartX ${Me.X}
   /varset ACState 0
   /varset ACMATarget 0
   /varset ADMobFound FALSE
   /varset ADCharmID 0
/return


Sub ResetBardGems
   /declare a int local
   /declare tTwistGems string local
   /if (${MBPause} && ${Twist.Twisting}) {
      /varset tTwistGems ${Twist.List}
      /for a 1 to ${Twist.List.Count[ ]}
         /if (${Me.Gem[${Twist.List.Arg[${a}]}].HasSPA[57]}) {
            /call EditTwist "${tTwistGems}" ${Twist.List.Arg[${a}]} delete
            /varset tTwistGems ${Macro.Return}
         }
      /next a
      /if (!${Twist.List.Equal[${tTwistGems}]}) {
         /if (${tTwistGems.Length}) {
            /squelch /twist ${tTwistGems}
         } else {
            /squelch /twist clear
         }
      }
   }
   /varset BardRestGems
   /varset BardCombatGems
   /call ABBuffCheck
/return


Sub Event_InvalidZone
   /varset bInvalidZone TRUE
   /doevents flush InvalidZone
/return

   
Sub ClearCursor(string ccParm)
   /if (!${Cursor.ID}) /return TRUE
   /if (${Window[InventoryWindow].Open} || ${Window[BigBankWnd].Open}) {
      /if (!${DoCombines} && !${ccParm.Find[force]}) /return FALSE
   }
   /declare x int local
   /declare tTimer timer local 100
   /while (${Cursor.ID}) {
      /if (${Cursor.Items}) {
         /for x 1 to ${Me.NumBagSlots}
            /if (!${Me.Inventory[pack${x}].ID}) {
               /nomodkey /itemnotify pack${x} leftmouseup
               /break
            }
         /next x
         /if (${Cursor.Container}) /break
      } else {
         /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Keeping - ${Cursor.Name}
         /autoinventory
         /delay 2s !${Cursor.ID}
         /delay 5
      }
      /delay 2
      /if (!${tTimer}) /break
   }
   /if (${Cursor.ID}) {
      /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Unable to clear the cursor - ${Cursor.Name}
      /return FALSE
   }
/return TRUE


Sub Buy(string sAmount, string iName, string mName)
   /declare bAmount int local
   /if (${sAmount.Find[${PipeChar}]}) {
      /varset bAmount ${sAmount.Arg[1,${PipeChar}]}
   } else {
      /varset bAmount ${sAmount}
   }
    
   /if (${bAmount}==0) {
      /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Buy amount not specified. Skipping Buy routine.
      /return 0
   }
   /if (!${iName.Length}) {
      /if (${Merchant.Open} && ${Merchant.SelectedItem.ID}) {
         /varset iName ${Merchant.SelectedItem.Name}
      } else {
         /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Buy item not specified. Skipping Buy routine.
         /return 0
      }
   }
   /if (!${mName.Length}) {
      /if (${Merchant.Open}) {
         /varset mName ${Merchant.CleanName}
      } else /if (${Target.ID} && ${Spawn[merchant id ${Target.ID}].ID}) {
         /varset mName ${Target.CleanName}
      } else /if (${Spawn[merchant los radius 19].ID}) {
         /varset mName ${Spawn[merchant los radius 19].CleanName}
      } else {
         /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Merchant not specified/found. Skipping Buy routine.
         /return 0
      }
   }

   /declare iAmount int local ${FindItemCount[=${iName}]}   
   /declare QTY int local 0
   /declare bMaxFlag bool local ${If[${sAmount.Arg[2,${PipeChar}].Equal[max]},TRUE,FALSE]}
   /declare eAmount int local 0
   /declare lAmount int local 0
   /declare pAmount int local 0
   /declare pTotAmount int local 0
   /declare pMaxAmount int local 0
   /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Buy request: ${bAmount} ${If[${bMaxFlag},(max final inventory) ,]}${iName} from ${mName}.
   /if (${iAmount}>=${bAmount} && ${bMaxFlag}) {
      /echo ${If[${TimeStampOnEcho},[${Time}] ,]}I already have ${iAmount} ${iName}. Skipping Buy routine.
      /return 0
   }
   /if (!${Spawn[merchant ${mName}].ID} || !${Spawn[merchant ${mName}].Distance}>19) {
      /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Merchant not found or too far away. Skipping Buy routine.
      /return 0
   }
   /tar npc ${mName}
   /delay 1s ${Target.ID}==${Spawn[merchant ${mName}].ID}
   /invoke ${Merchant.OpenWindow}
   /delay 5s ${Merchant.Open}
   /if (!${Merchant.Open}) {
      /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Failed to open merchant window. Skipping Buy routine.
      /return 0
   }
   /varset mName ${Merchant.CleanName}   
   /delay 5s ${Merchant.ItemsReceived}
   /if (!${Merchant.ItemsReceived}) {
      /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Failed to populate merchant window. Skipping Buy routine.
      /goto :BuyDone
   }   
   /invoke ${Merchant.SelectItem[=${iName}]}
   /if (!${Merchant.SelectedItem.Name.Equal[${iName}]}) {
      /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Failed to find ${iName} in Merchant's inventory. Skipping Buy routine.
      /goto :BuyDone
   }
   /if (${iAmount}) {
      /echo ${If[${TimeStampOnEcho},[${Time}] ,]}I already have ${iAmount} ${iName}.
   }
   /if (${bMaxFlag}) { 
      /varcalc QTY ${bAmount}-${iAmount}
   } else {
      /varset QTY ${bAmount}
   }
   /if (${Merchant.SelectedItem.MerchQuantity}!=-1 && ${Merchant.SelectedItem.MerchQuantity}<${QTY}) {
      /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Merchant only has ${Merchant.SelectedItem.MerchQuantity} ${iName} in inventory.
      /varset QTY ${Merchant.SelectedItem.MerchQuantity}
   }
   /varcalc pMaxAmount ${iAmount}+${QTY}
   /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Attempting to buy ${QTY} ${iName}.
   /doevents flush Broke
   /while (1) {
      /varset lAmount ${iAmount}
      /if (!${Merchant.SelectedItem.Name.Equal[${iName}]}) {
         /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Merchant has no ${iName} remaining in inventory.
         /break
      }
      /if (${Merchant.SelectedItem.MerchQuantity}!=-1 && ${Merchant.SelectedItem.MerchQuantity}<${QTY}) {
         /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Merchant only has ${Merchant.SelectedItem.MerchQuantity} ${iName} remaining in inventory.
         /varset QTY ${Merchant.SelectedItem.MerchQuantity}
      }
      /if (${QTY}<=0) /break
      /if (!${Merchant.SelectedItem.Container}) {
         /invoke ${Merchant.Buy[${QTY}]}
      } else {
         /invoke ${Merchant.Buy[1]}
      }
      /delay 1s
      /doevents Broke
      /varset iAmount ${FindItemCount[=${iName}]}
      /varcalc pAmount ${iAmount}-${lAmount}
      /if (!${pAmount}) {
         /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Failed to buy ${iName} from merchant.
         /break
      }
      /varcalc pTotAmount ${pTotAmount}+${pAmount}
      /if (${iAmount}>=${pMaxAmount}) /break
      /varcalc QTY ${pMaxAmount}-${iAmount}
   }
:BuyDone
   /nomodkey /notify MerchantWnd MW_Done_Button leftmouseup
   /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Bought ${pTotAmount} ${iName} from ${mName}. I now have ${FindItemCount[=${iName}]}.
/return ${pTotAmount}


Sub SellCheck(VendorName,P1)
   /if (!${VendorName.Length}) /return FALSE
   /declare tPack int local
   /declare tItem int local
   /declare tItemID int local
   /declare tItemName string local
   /declare mItem int local
   /declare mItemName string local
   /call ClearCursor force
   /squelch /target npc ${VendorName}
   /delay 3s ${Target.ID}==${Spawn[merchant ${VendorName}].ID}
   /invoke ${Merchant.OpenWindow}
   /delay 3s ${Merchant.Open}
   /if (!${Merchant.Open}) {
      /echo ${If[${TimeStampOnEcho},[${Time}] ,]}SellCheck error - Merchant Window failed to Open
      /return FALSE
   }
   /delay 5s ${Merchant.ItemsReceived}
   /if (!${Merchant.ItemsReceived}) {
      /echo ${If[${TimeStampOnEcho},[${Time}] ,]}SellCheck error - Merchant Window failed to Populate
      /return FALSE
   }
   /for tPack 1 to ${Me.NumBagSlots}
      /if (!${Me.Inventory[pack${tPack}].ID}) /continue
      /for tItem ${If[${Me.Inventory[pack${tPack}].Container},1,0]} to ${Me.Inventory[pack${tPack}].Container}
         /if (!${Me.Inventory[pack${tPack}]${If[${tItem},.Item[${tItem}],]}.ID}) /continue
         /varset tItemName ${Me.Inventory[pack${tPack}]${If[${tItem},.Item[${tItem}],]}.Name}
         /if (${Select[${Ini["${LootIni}","${tItemName.Left[1]}","${tItemName}"]},Sell${If[!${P1.Length},,,${P1}]}]}) {
            /varset tItemID ${Me.Inventory[pack${tPack}]${If[${tItem},.Item[${tItem}],]}.ID}
            /call Sell ${tItemID} ${Me.Inventory[pack${tPack}]${If[${tItem},.Item[${tItem}],]}.ItemSlot} ${Me.Inventory[pack${tPack}]${If[${tItem},.Item[${tItem}],]}.ItemSlot2} ${Me.Inventory[pack${tPack}]${If[${tItem},.Item[${tItem}],]}.Stack}
         }
      /next tItem
   /next tPack          
   /for mItem 1 to ${Merchant.Items}
      /varset mItemName ${Merchant.Item[${mItem}].Name}
      /if (${Ini["${LootIni}","${mItemName.Left[1]}","${mItemName}"].EqualCS[KEEP]}) {
         /bc Msg: ${mItemName} found on ${VendorName}
      }
   /next mItem
   /nomodkey /notify MerchantWnd MW_Done_Button leftmouseup
   /squelch /target clear
/return TRUE


Sub Sell(int iID, int iSlot, int iSlot2, int iCount)
   /if (!${iID} || !${Window[MerchantWnd].Open}) /return
   /declare tPack string local pack${Math.Calc[${iSlot}-22].Int}
   /declare tSlot int local ${If[${iSlot2}>=0,${Math.Calc[${iSlot2}+1].Int},0]}
   /declare tCash int local ${Me.Cash}
   /call ClearCursor force
   /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Selling: ${FindItem[${iID}].Name} - ${iCount}
   /nomodkey /itemnotify ${If[${tSlot},in ,]}${tPack} ${If[${tSlot},${tSlot} ,]}leftmouseup
   /delay 2s ${Merchant.SelectedItem.ID}==${iID}
   /if (${Merchant.SelectedItem.ID}==${iID} && ${Merchant.SelectedItem.SellPrice}) {
      /delay ${Math.Calc[${Math.Rand[1]}+1].Int}s
      /nomodkey /shift /notify MerchantWnd MW_Sell_Button leftmouseup
      /delay 2s (${Me.Cash}!=${tCash})
   }
   /if (${Me.Cash}==${tCash}) {
      /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Looks like we were unable to sell ${FindItem[${iID}].Name}
      /return FALSE
   }
/return TRUE


Sub GetBankItem(string ItemName,int Quant,string InStacks)
| We could make this 30, if we want to include Shared Bank. Or maybe a parm?
   /declare BankSlots int local 24
   /declare Gathered int local 0
   /declare NumItems int local 1
   /declare tStacks bool local TRUE
   /declare tBank int local
   /declare tItem int local
   /declare tItemID int local
   /declare tItemName string local
   /declare tBankWasOpen bool local FALSE

   /if (${Window[BigBankWnd].Open}) {
      /varset tBankWasOpen TRUE
   } else {
      /call OpenBank
      /if (${Macro.Return.Equal[false]}) /return FALSE
   }
   /if (!${ItemName.Length} || !${Window[BigBankWnd].Open}) {
      /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Error! Usage - Bank Window must be Open - getbank <ItemName> <Quantity> <NoStacks>
      /return
   }
|  /call OpenAllBankContainers ${BankSlots}
| Honestly, I don't see any purpose in NOT using stacks. Everything gets /autoinventory'd, so it will end up in stacks regardless.
| Unless we purposefully placed them into separate inventory slots. But why would we want to do that?
   /if (${InStacks.Length} && ${Select[${InStacks},false,no,nostacks]}) /varset tStacks FALSE
   /call ClearCursor force 
   /if (${Category.Find[${ItemName}]}) {
| Search for items marked as Category in loot.ini file.
      /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Searching your bank for items marked "${ItemName}" in your loot file.
      /for tBank 1 to ${BankSlots}
         /if (!${Me.Bank[${tBank}].ID}) /continue
         /for tItem ${If[${Me.Bank[${tBank}].Container},1,0]} to ${Me.Bank[${tBank}].Container}
            /varset tItemName ${Me.Bank[${tBank}]${If[${tItem},.Item[${tItem}],]}.Name}
            /if (${tItemName.NotEqual[NULL]} && ${Ini["${LootIni}","${tItemName.Left[1]}","${tItemName}"].Find[${ItemName}]}) {
               /varset tItemID ${Me.Bank[${tBank}]${If[${tItem},.Item[${tItem}],]}.ID}
               /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Found matching item: ${tItemName} (${tItemID}) - ${FindItemBankCount[${tItemID}]}
               /while (${FindItemBankCount[${tItemID}]}) {
                  /call PickUpBankItem ${tItemID} ${FindItemBank[${tItemID}].ItemSlot} ${FindItemBank[${tItemID}].ItemSlot2} ${FindItemBank[${tItemID}].Stack}
                  /if (${Macro.Return.Equal[false]}) {
                     /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Problem with GetBankItem - PickUpBankItem failed
                     /return FALSE
                  }
                  /varcalc Gathered ${Gathered}+${Cursor.Stack}
                  /autoinventory
                  /delay 2s !${Cursor.ID}
                  /if (${Cursor.ID}) {
                     /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Problem with GetBankItem - Autoinventory Failed
                     /return FALSE
                  }
               }
            }
         /next tItem
      /next tBank 
      /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Gathered ${Gathered} items marked "${ItemName}" from Bank.
   } else {
      /varset tItemID ${FindItemBank[=${ItemName}].ID}
      /if (!${tItemID}) {
         /echo ${If[${TimeStampOnEcho},[${Time}] ,]}No Items found in your bank with name: ${ItemName}.
         /return FALSE
      }
      /varset NumItems ${If[${Quant}==0,${FindItemBankCount[${tItemID}]},${Quant}]}
      /while (${NumItems}>0 && ${FindItemBankCount[${tItemID}]}) {
         /call PickUpBankItem ${tItemID} ${FindItemBank[${tItemID}].ItemSlot} ${FindItemBank[${tItemID}].ItemSlot2} ${If[!${tStacks},1,${If[${FindItemBank[${tItemID}].Stack}<=${NumItems},${FindItemBank[${tItemID}].Stack},${NumItems}]}]}
         /if (${Macro.Return.Equal[false]}) {
            /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Problem with GetBankItem - PickUpBankItem failed
            /return FALSE
         }
         /varcalc Gathered ${Gathered}+${Cursor.Stack}
         /varcalc NumItems ${NumItems}-${Cursor.Stack}
         /autoinventory
         /delay 2s !${Cursor.ID}
         /if (${Cursor.ID}) {
            /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Problem with GetBankItem - Autoinventory Failed
            /return FALSE
         }
      }
      /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Gathered ${Gathered} "${ItemName}" from Bank.
   }
   /if (!${tBankWasOpen}) {
      /delay 1
      /nomodkey /notify BigBankWnd BIGB_DoneButton leftmouseup
      /delay 5
   }
/return TRUE


Sub OpenAllBankContainers(int BankSlots)
   /declare i int local 1
   /for i 1 to ${BankSlots} 
      /if (${Int[${Me.Bank[${i}].Container}]}) { 
         /if (!${Window[bank${i}].Open}) { 
            /nomodkey /itemnotify bank${i} rightmouseup 
            /delay 5s ${Window[bank${i}].Open} 
         } 
         /if (!${Window[bank${i}].Open}) /echo ${If[${TimeStampOnEcho},[${Time}] ,]}ERROR -- Possible problem opening bank container at # ${i}. 
         /delay 2 
      } 
   /next i 
/return 1 


Sub OpenBank
   /if (${Window[BigBankWnd].Open}) /return TRUE
   /declare a int local 0
   /for a 1 to 5
      /if (${Window[BigBankWnd].Open}) /break
      /if (!${Target.Class.Name.Equal[Banker]}) {
         /squelch /target clear
         /squelch /target id ${NearestSpawn[banker los radius 19].ID}
         /delay 1s ${Target.ID}
         /if (!${Target.Class.Name.Equal[Banker]}) {
            /stick 15
            /delay 5 ${Me.Moving}
            /delay 10 !${Me.Moving}
            /squelch /face nolook ${If[${MacroQuest.BuildName.Equal[emu]},fast ,]}
            /delay 5
            /continue
         }
      }
      /nomodkey /click right target
      /delay 1s ${Window[BigBankWnd].Open}
      /if (!${Target.Class.Name.Equal[Banker]} && !${Window[BigBankWnd].Open}) {
|        /echo Add strafing code to OpenBank Sub
         /stick off
         /delay ${Math.Calc[2+${Math.Rand[20]}]}
         /if (${Math.Rand[2]} < 2) {
            /nomodkey /keypress strafe_left hold
            /delay 2
            /nomodkey /keypress strafe_left
         } else {
            /nomodkey /keypress strafe_right hold
            /delay 2
            /nomodkey /keypress strafe_right
         }
      }
   /next a
   /if (!${Window[BigBankWnd].Open}) {
      /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Failed to open Bank Window
      /return FALSE
   }
/return TRUE


Sub BankThemAll(int tItemID)
   /if (!${tItemID}) /return
   /while (${FindItemCount[${tItemID}]}) {
      /call PickUpItem ${tItemID} ${FindItem[${tItemID}].ItemSlot} ${FindItem[${tItemID}].ItemSlot2} ${FindItem[${tItemID}].Stack}
      /if (${Macro.Return.Equal[false]}) {
         /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Problem with Banking - PickUpItem failed
         /return FALSE
      }
      /nomodkey /notify BigBankWnd bigb_autobutton leftmouseup 
      /delay 2s !${Cursor.ID}
      /if (${Cursor.ID}) {
         /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Failed to Bank Item - ${Cursor.Name}
         /return FALSE
      }
      /delay 1
   }
/return TRUE


Sub GiveCheck(string tTarget,string ItemNames,string ItemCount,string InStacks,string PerTradeCount)
|  /echo g1 ${tTarget} : ${ItemNames} : ${ItemCount} : ${InStacks} : ${PerTradeCount}
   /if (!${tTarget.Length} || !${Spawn[${tTarget}].ID} || !${ItemNames.Length}) /return FALSE
   /declare tTargetID int local ${Spawn[${tTarget}].ID}
   /declare a int local
   /declare b int local
   /declare NumItems int local 1
   /declare tPack int local
   /declare tItem int local
   /declare tItemID int local
   /declare tItemName string local
   /declare tStacks bool local TRUE
   /declare tPTCount int local 0

   /varset gX ${Me.X.Int}
   /varset gY ${Me.Y.Int}
   /varset gCount 0

   /if (!${ItemCount.Length}) /varset ItemCount 1
   /varset a ${Math.Calc[${ItemNames.Count[${PipeChar}]}+1]}
   /varset b ${Math.Calc[${ItemCount.Count[${PipeChar}]}+1]}
   /if (${b}>${a} || (${b}>1 && ${b}!=${a})) {
      /echo ${If[${TimeStampOnEcho},[${Time}] ,]}GiveCheck Parameter mismatch - Aborting
      /return FALSE
   }
   /if (${InStacks.Length} && ${Select[${InStacks},false,no,nostacks]}) /varset tStacks FALSE
   /if (${PerTradeCount.Length}) /varset tPTCount ${PerTradeCount}
| Set to 99 to cover Max Give(4) and Max Trade(8), and to signify Max
   /if (${tPTCount}<1 || ${tPTCount}>8) /varset tPTCount 99
|/echo g2 ${tTargetID} : ${a} : ${b} : ${tStacks} : ${tPTCount}
   /if (${Spawn[${tTarget}].Distance3D}>100) {
      /echo ${If[${TimeStampOnEcho},[${Time}] ,]}GiveCheck - Target is too far away
      /return FALSE
   }
   /target id ${tTargetID}
   /delay 2s ${Target.ID}
   /if (${Target.ID}!=${tTargetID}) {
      /echo ${If[${TimeStampOnEcho},[${Time}] ,]}GiveCheck - Failed to Obtain Give Target
      /return FALSE
   }
   /if (${Target.Distance3D}>12) {
      /call MBMoveTo ${Target.Y} ${Target.X}
      /if (!${Macro.Return.Equal[TRUE]}) {
         /echo ${If[${TimeStampOnEcho},[${Time}] ,]}GiveCheck - Problem getting to ${Spawn[${tTarget}].CleanName}
         /return FALSE
      }
   }
| Trading with a PC will always stack, even if given idividually. So, just stack them.   
   /if (${Target.Type.Equal[pc]}) /varset tStacks TRUE   
   /call ClearCursor force
   /if (${Target.ID}!=${tTargetID}) {
      /squelch /target id ${tTargetID}
      /delay 1s ${Target.ID}==${tTargetID}
   }
|  /nomodkey /keypress OPEN_INV_BAGS
|  /delay 2s

   /for a 1 to ${Math.Calc[${ItemNames.Count[${PipeChar}]}+1]}
      /if (${Category.Find[${ItemNames.Arg[${a},${PipeChar}]}]}) {
| Search for items marked as Category in loot.ini file.
         /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Searching inventory for items marked ${ItemNames.Arg[${a},${PipeChar}]} in your loot file.
         /for tPack 1 to ${Me.NumBagSlots}
            /if (!${Me.Inventory[pack${tPack}].ID}) /continue
            /for tItem ${If[${Me.Inventory[pack${tPack}].Container},1,0]} to ${Me.Inventory[pack${tPack}].Container}
               /varset tItemName ${Me.Inventory[pack${tPack}]${If[${tItem},.Item[${tItem}],]}.Name}
               /if (${tItemName.NotEqual[NULL]} && ${Ini["${LootIni}","${tItemName.Left[1]}","${tItemName}"].Find[${ItemNames.Arg[${a},${PipeChar}]}]}) {
                  /varset tItemID ${Me.Inventory[pack${tPack}]${If[${tItem},.Item[${tItem}],]}.ID}
                  /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Found matching item: ${tItemName} (${tItemID})
                  /call PickUpItem ${tItemID} ${Me.Inventory[pack${tPack}]${If[${tItem},.Item[${tItem}],]}.ItemSlot} ${Me.Inventory[pack${tPack}]${If[${tItem},.Item[${tItem}],]}.ItemSlot2} ${Me.Inventory[pack${tPack}]${If[${tItem},.Item[${tItem}],]}.Stack}
                  /if (${Macro.Return.Equal[false]}) {
                     /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Problem with GiveCheck - PickUpItem failed
                     /call GiveCleanup
                     /return FALSE
                  }
                  /call Give ${tItemID} ${tTargetID} ${tPTCount}
                  /if (${Macro.Return.Equal[false]}) {
                     /call GiveCleanup
                     /return FALSE
                  }
               }
            /next tItem
         /next tPack       
      } else {
         /varset tItemID ${FindItem[=${ItemNames.Arg[${a},${PipeChar}]}].ID}
         /if (!${tItemID}) {
            /echo ${If[${TimeStampOnEcho},[${Time}] ,]}No Items found with name: ${ItemNames.Arg[${a},${PipeChar}]}. Skipping.
            /break
         }
         /varset NumItems ${If[${b}==1,${If[${ItemCount.Find[all]},${FindItemCount[${tItemID}]},${ItemCount}]},${If[${ItemCount.Arg[${a},${PipeChar}].Find[all]},${FindItemCount[${tItemID}]},${ItemCount.Arg[${a},${PipeChar}]}]}]}
         /while (${NumItems}>0 && ${FindItemCount[${tItemID}]}) {
            /call PickUpItem ${tItemID} ${FindItem[${tItemID}].ItemSlot} ${FindItem[${tItemID}].ItemSlot2} ${If[!${tStacks},1,${If[${FindItem[${tItemID}].Stack}<=${NumItems},${FindItem[${tItemID}].Stack},${NumItems}]}]}
            /if (${Macro.Return.Equal[false]}) {
               /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Problem with GiveCheck - PickUpItem failed
               /call GiveCleanup
               /return FALSE
            }
            /varcalc NumItems ${NumItems}-${Cursor.Stack}
            /call Give ${tItemID} ${tTargetID} ${tPTCount}
            /if (${Macro.Return.Equal[false]}) {
               /call GiveCleanup
               /return FALSE
            }
|/echo g4a ${NumItems}
         }
|/echo g4b ${NumItems}
      }
      /if (${gCount} && ${gCount}<${tPTCount} && ${tPTCount}!=99) {
         /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Not enough for Per-Trade amount. Cancelling trade.
         /call CancelGive
         /delay 2
         /echo ${If[${TimeStampOnEcho},[${Time}] ,]}I now have ${FindItemCount[=${ItemNames.Arg[${a},${PipeChar}]}]} ${ItemNames.Arg[${a},${PipeChar}]} on me.
      } else {
         /call DoGive
      }
   /next a
   /call GiveCleanup
/return TRUE

Sub ResetBag
   /declare x int local 0
   /declare y int local 0
   /keypress open_inv_bags
   /for x 1 to 7
      /for y 1 to ${InvSlot[Pack${x}].Item.Container}
         /if (${InvSlot[Pack${x}].Item.Item[${y}].Container}) {
            /if (!${Window[pack${x}].Open}) {
               /keypress open_inv_bags
               /delay 1s ${Window[pack${x}].Open}
            }
            /delay 1s ${Window[pack${x}].Open}
         		/echo /itemnotify in pack${x} ${y} leftmouseup
:reclick
            /itemnotify in pack${x} ${y} leftmouseup
            /delay 1s ${Cursor.ID}
            /if (!${Cursor.ID}) /goto :reclick
            /autoinv
            /autoinventory
            /return
         }
      /next y
   /next x
/return


Sub Give(int iItemID,int tID,int tPTCount)
   /if (${Cursor.ID}!=${iItemID}) {
      /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Problem with Give - Cursor check
      /return FALSE
   }
   /if (!${tID}) {
      /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Problem with Give - No Target
      /return FALSE
   }
   /if (${Target.ID}!=${tID}) {
      /squelch /target id ${tID}
      /delay 1s ${Target.ID}==${tID}
      /if (${Target.ID}!=${tID}) {
         /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Problem with Give - Target check
         /return FALSE
      }
   }
   /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Giving ${Cursor.Stack} ${Cursor.Name} -to- ${Target.CleanName}
   /if (${Target.ID}) /click left target
   /delay 10s (${Window[GiveWnd].Open} || ${Window[TradeWnd].Open})
   /if ((!${Target.Type.Equal[pc]} && !${Window[GiveWnd].Open}) || (${Target.Type.Equal[pc]} && !${Window[TradeWnd].Open})) {
      /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Problem with Give - Window/type check
      /return FALSE
   }
   /varcalc gCount ${gCount}+1
   /if (${Window[GiveWnd].Open}) {
      /if (${gCount}<4 && ${gCount}<${tPTCount}) {
         /return TRUE
      }
      /call DoGive
   } else /if (${Window[TradeWnd].Open}) {
      /if (${gCount}<8 && ${gCount}<${tPTCount}) {
         /return TRUE
      }
      /call DoGive
   }
   /varset gCount 0
/return TRUE


Sub DoGive
   /if (${gCount}) {
|/echo dg ${gCount}
      /if (${Window[GiveWnd].Open}) {
         /nomodkey /notify GiveWnd GVW_Give_Button LeftMouseUp
         /delay 5s !${Window[GiveWnd].Open}
      }
      /if (${Window[TradeWnd].Open}) {
         /if (!${Window[TradeWnd].MyTradeReady}) {
            /delay 2
            /nomodkey /notify TradeWnd TRDW_Trade_Button LeftMouseUp
         }
         /delay 15s !${Window[TradeWnd].Open}
      } 
   }
/return


Sub CancelGive
   /if (${Window[GiveWnd].Open}) {
      /nomodkey /notify GiveWnd GVW_Cancel_Button LeftMouseUp
      /delay 5s !${Window[GiveWnd].Open}
   }
   /if (${Window[TradeWnd].Open}) {
      /nomodkey /notify TradeWnd TRDW_Cancel_Button LeftMouseUp
      /delay 5s !${Window[TradeWnd].Open}
   }
   /varset gCount 0   
/return


Sub GiveCleanup
   /nomodkey /keypress CLOSE_INV_BAGS
   /nomodkey /keypress esc
   /delay 1s
   /if (${Me.X.Int}!=${gX} || ${Me.Y.Int}!=${gY}) /call MBMoveTo ${gY} ${gX}
/return


Sub PickUpItem(int iID, int iSlot, int iSlot2, int iCount)
   /declare tPack string local pack${Math.Calc[${iSlot}-22].Int}
   /declare tSlot int local ${If[${iSlot2}>=0,${Math.Calc[${iSlot2}+1].Int},0]}
   /if (${iCount}==${Me.Inventory[${tPack}]${If[${tSlot},.Item[${tSlot}],]}.Stack}) {
      /nomodkey /shift /itemnotify ${If[${tSlot},in ,]}${tPack} ${If[${tSlot},${tSlot} ,]}leftmouseup
   } else {
      /if (${tSlot}>0 && !${Me.Inventory[${tPack}].Open}) {
         /nomodkey /itemnotify ${tPack} rightmouseup
         /delay 2s ${Me.Inventory[${tPack}].Open}
         /delay 5
      }
      /nomodkey /itemnotify ${If[${tSlot},in ,]}${tPack} ${If[${tSlot},${tSlot} ,]}leftmouseup
      /delay 2s ${Window[QuantityWnd].Open}
      /if (!${Window[QuantityWnd].Open}) {
         /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Problem with PickUpItem - Quantity Window check
         /return FALSE
      }
      /nomodkey /notify QuantityWnd QTYW_Slider newvalue ${iCount}
      /delay 5
      /nomodkey /notify QuantityWnd QTYW_Accept_button leftmouseup
   }
   /delay 2s ${Cursor.ID}==${iID}
   /delay 5
   /if (${Cursor.ID}!=${iID} || ${Cursor.Stack}!=${iCount}) {
      /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Problem with PickUpItem - Cursor check
      /return FALSE
   }
/return TRUE


Sub PickUpBankItem(int iID, int iSlot, int iSlot2, int iCount)
   /declare tBank int local ${Math.Calc[${iSlot}+1].Int}
   /declare sBank string local bank${Math.Calc[${iSlot}+1].Int}
   /declare tSlot int local ${If[${iSlot2}>=0,${Math.Calc[${iSlot2}+1].Int},0]}
   /if (${iCount}==${Me.Bank[${tBank}]${If[${tSlot},.Item[${tSlot}],]}.Stack}) {
      /nomodkey /shift /itemnotify ${If[${tSlot},in ,]}${sBank} ${If[${tSlot},${tSlot} ,]}leftmouseup
   } else {
      /if (${tSlot}>0 && !${Me.Bank[${tBank}].Open}) {
         /nomodkey /itemnotify ${sBank} rightmouseup
         /delay 2s ${Me.Bank[${tBank}].Open}
         /delay 5
      }
      /nomodkey /itemnotify ${If[${tSlot},in ,]}${sBank} ${If[${tSlot},${tSlot} ,]}leftmouseup
      /delay 2s ${Window[QuantityWnd].Open}
      /if (!${Window[QuantityWnd].Open}) {
         /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Problem with PickUpBankItem - Quantity Window check
         /return FALSE
      }
      /nomodkey /notify QuantityWnd QTYW_Slider newvalue ${iCount}
      /delay 5
      /nomodkey /notify QuantityWnd QTYW_Accept_button leftmouseup
   }
   /delay 2s ${Cursor.ID}==${iID}
   /if (${Cursor.ID}!=${iID} || ${Cursor.Stack}!=${iCount}) {
      /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Problem with PickUpBankItem - Cursor check
      /return FALSE
   }
   /delay 5
/return TRUE


Sub MBScript
   /declare a int local
   /declare b int local
   /declare c int local
   /declare Timer timer local
   /declare sVarL string local
   /declare sVarC string local
   /declare sVarM string local
   /declare ScriptINI string local ${IniFile}
   /declare sPrefix string local ${

   | /echo Param0 : ${ScriptINI} : ${Param0} : ${NetworkINI}
   /if (!${Defined[Param0]}) {
      /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Syntax Error - Script Name Missing
      /return
   }  
   /if (!${Param0.Length}) {
      /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Syntax Error - Script Name Missing
      /return
   }
   /if (${Select[${Param0.Arg[1,-]},net,network]}) {
      /varset ScriptINI ${NetworkINI}
      /if (!${Param0.Find[ ]}) {
         /varset Param0 ${Param0.Right[${Math.Calc[${Param0.Length}-${Param0.Find[-]}]}]}
      } else {
         /varset Param0 ${Param0.Arg[2]}
      }
   }
   |/echo Param0 : ${ScriptINI} : ${Param0} : ${NetworkINI}
   /if (!${Defined[${Param0}C]}) {
      /declare ${Param0}C int outer ${Ini["${ScriptINI}",Script-${Param0},Commands]}
      /if (!${${Param0}C}) {
         /declare IniString string local ${Ini["${ScriptINI}",Script-${Param0},Commands,NOTFOUND]}
         /if (${IniString.Equal[NOTFOUND]}) {
            /ini "${ScriptINI}" "Script-${Param0}" "Commands" "0"
            /ini "${ScriptINI}" "Script-${Param0}" "C1" "/return"
            /if (!${Select[${Param0},MBStartup,Defense]}) {
               /echo ${If[${TimeStampOnEcho},[${Time}] ,]}MBScript - INI Entry Made for: Script-${Param0}
            }
         }
         /return
      }
      /declare ${Param0}[${${Param0}C}] string outer
      /for a 1 to ${${Param0}C}
         /echo c${a}
         /varset ${Param0}[${a}] ${Ini["${ScriptINI}",Script-${Param0},C${a}]}
      /next a
   }
   /if (!${${Param0}C}) /return
   /if (${Param0.Equal[Defense]}) {
      /if (${Defined[Param1]}) {
         /if (${Param1.Equal[Startup]}) /return
      }
   }
   /varset Timer 15s
| The ":Top" label is here for Script writers to use if they want it.
:Top
   /for a 1 to ${${Param0}C}
| Replace all { with ${
      /if (!${${Param0}[${a}].Count[{]}) {
         /varset sVarC ${${Param0}[${a}]}
      } else {
         /varset sVarC
         /for b 1 to ${${Param0}[${a}].Count[{]}
            /varset sVarL ${${Param0}[${a}].Arg[${b},{]}
            /varset sVarC ${sVarC}${sVarL}${sPrefix}
         /next b
         /varset sVarC ${sVarC}${${Param0}[${a}].Arg[${b},{]}
      }
| Replace all '' with " (pairs of single quotes with a double-quote)
      /varset sVarM
      /while (${sVarC.Find['']}) {
         /varset sVarL ${sVarC.Mid[1,${Math.Calc[${sVarC.Find['']}-1]}]}"
         /varset sVarC ${sVarC.Mid[${Math.Calc[${sVarC.Find['']}+2]},${sVarC.Length}]}
         /varset sVarM ${sVarM}${sVarL}
      }
      /if (${sVarM.Length}) {
         /varset sVarC ${sVarM} ${sVarC}
      }
      /if (${sVarC.Length} && ${sVarC.NotEqual[NULL]}) {
         /if (${EchoScripts}) {
            /echo ${If[${TimeStampOnEcho},[${Time}] ,]}/docommand ${sVarC}
         }
         /docommand ${sVarC}
      }
      /if (!${Timer}) {
         /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Script [${Param0}] took too long, exiting
         /return
      }
   /next a
/return


Sub CastCall(CCMsgFrom,P1)
   /declare CCMsgTxt string local ${If[${P1.Length},${P1},${CommandText}]}
|/echo Msg: ${CCMsgFrom} - ${CCMsgTxt}
   /declare a int local 1
   /declare b int local 1
   /declare c int local 1
   /declare d int local 1
   /declare amax int local 5
   /declare bmin int local 1
   /declare bmax int local 1
   /declare cmax int local 1
   /if (${CCMsgTxt.Arg[1].Equal[coresist]}) {
      /declare tCastClass string local AD AH AB AQ AE
   } else {
      /declare tCastClass string local AH AD AB AQ AE
   } 
   /declare tCastCnt int local 0
   /declare tGrpMbr int local 0
   /declare tNBMember int local 0   
   /declare sGem string local
   /declare sReturn string local
   /declare tGem int local 0
   /declare tSpell string local
   /declare tSpellID int local 0
   /declare tSpellRange int local 0
   /declare tCastTime timer local 0
   /declare tCT int local
   /declare tTargetID int local 0
   /declare CCtTargetID int local 0
   /declare tCCMsgTxt string local
   /declare ProjectIllu bool local FALSE
   /declare tFoci string local
   /declare CastCount int local 0
   /declare tCCMsgArg1 string local ${CCMsgTxt.Arg[1]}
   /declare tTwistGems string local
   /declare tSCast bool local FALSE
   /declare tPrevTarget int local
   /declare tMGB bool local FALSE
   /declare tMGBName string local
   
   /if (${Me.Class.ShortName.Equal[brd]}) {
      /varset tTwistGems ${Twist.List}
   }
   /if (${CCMsgTxt.Arg[1].Equal[scast]}) {
      /varset tCastClass ${CCMsgTxt.Arg[2].Arg[1,${PipeChar}]}
      /if (!${Select[${tCastClass},AD,AH,AB,AQ,AE]}) {
         /echo ${If[${TimeStampOnEcho},[${Time}] ,]}CastCall - Bad Class on scast - ${tCastClass}
         /return
      }
      /varset amax 1
      /varset sGem ${CCMsgTxt.Arg[2].Arg[2,${PipeChar}]}
      /if (!${${tCastClass}Gem[${sGem}].Length}) {
         /echo ${If[${TimeStampOnEcho},[${Time}] ,]}CastCall - Bad Item on scast - ${sGem}
         /return
      }
      /varset bmin ${sGem}
      /varset bmax ${sGem}
      /varset cmax 1
      /varset tSCast TRUE
   } else /if (${Select[${CCMsgTxt.Arg[1]},mgb,bomb,tmgb,tbomb]}) {
      /call CheckMGB ${CCMsgTxt.Arg[1]}
      /if (${Macro.Return.Equal[FALSE]}) /return
      /if (${Macro.Return.Length}) {
         /varset tMGB TRUE
         /varset tMGBName ${Macro.Return}
      } else {
         /echo ${If[${TimeStampOnEcho},[${Time}] ,]}MGB AA is not available. I can't MGB ${CCMsgTxt.Arg[2]} right now.
         /bc Msg: MGB AA is not available. I can't MGB ${CCMsgTxt.Arg[2]} right now.
         /return
      }
   }
:AliasLoop
   /varset tGrpMbr 0
   /varset tNBMember 0   
   /varset CastCount 0    
   /for a 1 to ${amax}
      /if (${${tCastClass.Arg[${a}]}Count}) {
         /if (!${tSCast}) {
            /varset bmax ${${tCastClass.Arg[${a}]}Count}
         }
         /for b ${bmin} to ${bmax}
            /if (!${tSCast}) {
               /varset cmax ${Math.Calc[${${tCastClass.Arg[${a}]}SpellAlias[${b}].Count[${PipeChar}]}+1]}
            }
            /for c 1 to ${cmax}
               /if (${tSCast} || ${CCMsgTxt.Arg[2].Equal[${${tCastClass.Arg[${a}]}SpellAlias[${b}].Arg[${c},${PipeChar}]}]}) {
                  /varset sReturn ${tCastClass.Arg[${a}]} ${b}
                  /varset tSpell ${${tCastClass.Arg[${a}]}Spell[${b}]}
                  /varset sGem ${${tCastClass.Arg[${a}]}Gem[${b}]}
                  /varset tFoci ${${tCastClass.Arg[${a}]}SpellFoci[${b}]}
                  /if (${tCastClass.Arg[${a}].NotEqual[ab]}) {
                     /if (!${MasterList.Find[${CCMsgFrom}]} && ${CCMsgFrom.NotEqual[${Me.CleanName}]} && (!${MasterList.Equal[Netbots]} && ${NetBots.Client.Find[${CCMsgFrom}]})) /return
                  }
                  /if (${CCMsgTxt.Arg[3].Equal[off]}) {
                     /varset ${tCastClass.Arg[${a}]}TarCnt[${b}] 0
                     /bc Msg: ${CCMsgTxt.Arg[2]}-${tSpell} is Off
                     /if (${b}<${bmax}) {
                        /varcalc CastCount ${CastCount}+1
                        /continue
                     }
                     /if (${Me.Class.ShortName.NotEqual[brd]}) /return SUCCESS
                     /varset CCMsgTxt sung ${${tCastClass.Arg[${a}]}SpellAlias[${b}].Arg[${c},${PipeChar}]}
                  }
                  /if (${CCMsgTxt.Arg[3].Equal[on]}) {
                     /if (${MasterList.Find[${CCMsgFrom}]} || ${MasterList.Find[Netbots]} && ${NetBots[${CCMsgFrom}].ID} || ${CCMsgFrom.Equal[${Me.CleanName}]}) {
                        /if (${${tCastClass.Arg[${a}]}NewFormat}) {
                           /call LoadINIVar ${tCastClass.Arg[${a}]}${b} TarCnt 0 ${tCastClass.Arg[${a}]}TarCnt[${b}] ${IniFile}
                        } else {
                           /call LoadINIVar ${If[${tCastClass.Arg[${a}].Equal[ah]},AdvHeal,${If[${tCastClass.Arg[${a}].Equal[ab]},AdvBuff,${If[${tCastClass.Arg[${a}].Equal[ad]},AdvDebuff,${If[${tCastClass.Arg[${a}].Equal[ae]},AdvEvent,${If[${tCastClass.Arg[${a}].Equal[aq]},AdvCure,NOTFOUND]}]}]}]}]} ${tCastClass.Arg[${a}]}TarCnt${b} 0 ${tCastClass.Arg[${a}]}TarCnt[${b}] ${IniFile}
                        }
                        /if (${${tCastClass.Arg[${a}]}TarCnt[${b}]}==0) /varset ${tCastClass.Arg[${a}]}TarCnt[${b}] 1
                        /bc Msg: ${CCMsgTxt.Arg[2]}-${tSpell} is On
                        /if (${b}<${bmax}) {
                           /varcalc CastCount ${CastCount}+1
                           /continue
                        }
                        /return SUCCESS
                     }
                  }
                  /if (${Select[${CCMsgTxt.Arg[2]},mgb,bomb,tmgb,tbomb]}) {
                     /call CheckMGB ${CCMsgTxt.Arg[2]}
                     /if (${Macro.Return.Length} && ${Macro.Return.NotEqual[FALSE]}) {
                        /varset tMGB TRUE
                        /varset tMGBName ${Macro.Return}
                     } else {
                        /goto :CantCast
                     }
                  }
                  /varset tGem 0
                  /if (${Select[${sGem},${ValidSpellGems}]}) {
|Spell
                     /varset tSpellID ${Me.Book[${Me.Book[${tSpell}]}].ID}
                     /if (${Select[${tSpellID},${SpellsRestrictedByZone}]}) /goto :CantCast
                     /if (${Spell[${tSpellID}].Location}==1 && !${Zone.Outdoor}) /goto :CantCast
                     /if (!${tMGB}) {
                        /if (${Me.CurrentMana}<${Spell[${tSpellID}].Mana}) /goto :CantCast
                     } else {
                        /if (!${Spell[${tSpellID}].CanMGB}) /goto :CantCast
                        /if (${Me.CurrentMana}<${Math.Calc[${Spell[${tSpellID}].Mana}*2]}) /goto :CantCast
                     }
                     /varset tGem ${sGem}
                     /varcalc tCT ${Spell[${tSpellID}].MyCastTime}/100+50.99
                     /if (${Me.Casting.ID} && ${Me.Class.ShortName.NotEqual[BRD]}) /delay 2s !${Me.Casting.ID}
                     /if (!${Me.Gem[${tGem}].Name.Length} || ${Me.Gem[${tGem}].Name.NotEqual[${tSpell}]}) {
                        /call MemGem ${tGem} "${tSpell}"
                        /if (${Macro.Return.NotEqual[TRUE]}) {
                           /goto :CantCast
                        }
                        /varcalc tCT ${Spell[${tSpellID}].RecastTime}/100+${tCT}
                     } else /if (!${Cast.Ready[${tSpellID}${PipeChar}gem${tGem}]}) {
                        /varcalc tCT ${Spell[${tSpellID}].RecastTime}/100+${tCT}
                     }
                     /if (${Me.Class.ShortName.Equal[brd]}) {
                        /if (!${CCMsgTxt.Find[sing]} && !${CCMsgTxt.Find[sung]} && (${MBPause} || !${tTwistGems.Length})) {
                           /varset tTwistGems ${tGem}
                        } else {
                           /if (${CCMsgTxt.Find[sing]}) {
                              /varset ${sReturn.Arg[1]}TarCnt[${sReturn.Arg[2]}] 1
                              /if (${Spell[${tSpellID}].SpellType.Equal[detrimental]}) {
                                 /call EditTwist "${BardCombatGems}" ${tGem} add
                                 /varset BardCombatGems ${Macro.Return}
                              } else {
                                 /call EditTwist "${BardRestGems}" ${tGem} add
                                 /varset BardRestGems ${Macro.Return}
                              }
                           } else /if (${CCMsgTxt.Find[sung]}) {
                              /varset ${sReturn.Arg[1]}TarCnt[${sReturn.Arg[2]}] 0
                              /call EditTwist "${BardCombatGems}" ${tGem} delete
                              /varset BardCombatGems ${Macro.Return}
                              /call EditTwist "${BardRestGems}" ${tGem} delete
                              /varset BardRestGems ${Macro.Return}
                              /call EditTwist "${tTwistGems}" ${tGem} delete
                              /varset tTwistGems ${Macro.Return}
                              /varset tSpell Removed [${Spell[${tSpellID}].Name}] from list and Tarcnt=0(off).
                              /varcalc tCT 0
                              /goto :tContCast
                           }
                        }
                     }
                  } else /if (${sGem.Equal[item]} && ${FindItem[=${tSpell}].ID}) {
|Item
                     /varset tSpellID ${FindItem[=${tSpell}].Spell.ID}
                     /if (${Select[${tSpellID},${SpellsRestrictedByZone}]}) /goto :CantCast
                     /if (${Spell[${tSpellID}].Location}==1 && !${Zone.Outdoor}) /goto :CantCast
                     /if (${FindItem[=${tSpell}].Timer}) /goto :CantCast
                     /if (${tMGB}) {
                        /if (!${Spell[${tSpellID}].CanMGB}) /goto :CantCast
                     }
                     /varset tGem 20
                     /varcalc tCT ${FindItem[=${tSpell}].CastTime}/100+50.99
                     /if (${tSpell.Equal[Fellowship Registration Insignia]}) /makemevisible
                     /if (${Me.Class.ShortName.Equal[BRD]}) {
                        /if (!${${tCastClass.Arg[${a}]}BardSlot[${b}]}) {
                           /call GetBardSlot "${${tCastClass.Arg[${a}]}Spell[${b}]}" ${tCastClass.Arg[${a}]}${PipeChar}${b}
                           /if (!${${tCastClass.Arg[${a}]}BardSlot[${b}]}) {
                              /echo No BardSlot assigned. Can't cast Item: ${${tCastClass.Arg[${a}]}Spell[${b}]}
                              /goto :CantCast
                           }
                        }
                     }
                  } else /if (${sGem.Equal[alt]} && ${Me.AltAbility[${tSpell}].ID}) {
|Alt
                     /varset tSpellID ${Me.AltAbility[${tSpell}].Spell.ID}
                     /if (!${Me.AltAbilityReady[${tSpell}]}) /goto :CantCast
                     /if (${Select[${tSpellID},${SpellsRestrictedByZone}]}) /goto :CantCast
                     /if (${tMGB}) {
                        /if (!${Spell[${tSpellID}].CanMGB}) /goto :CantCast
                     }
                     /varset tGem 30
                     /varcalc tCT ${Me.AltAbility[${tSpell}].Spell.CastTime}/100+20.99
                     /if (${Me.Class.ShortName.Equal[BRD]}) {
                        /if (!${${tCastClass.Arg[${a}]}BardSlot[${b}]} && ${Spell[${tSpellID}].MyCastTime}) {
                           /call GetBardSlot "${${tCastClass.Arg[${a}]}Spell[${b}]}" ${tCastClass.Arg[${a}]}${PipeChar}${b}
                           /if (!${${tCastClass.Arg[${a}]}BardSlot[${b}]}) {
                              /echo No BardSlot assigned. Can't cast AA: ${${tCastClass.Arg[${a}]}Spell[${b}]}
                              /goto :CantCast
                           }
                        }
                     }
                  } else /if (${sGem.Equal[script]}) {
|Script
                     /call MBScript "${tSpell}"
                     /return
                  } else /if (${sGem.Equal[disc]} && ${Me.CombatAbility[${tSpell}]}) {
|Disc
                     /varset tSpellID ${Spell[${tSpell}].ID}
                     /if (!${Me.CombatAbilityReady[${tSpell}]}) /goto :CantCast
                     /if (!${tMGB}) {
                        /if (${Me.CurrentEndurance}<${Spell[${tSpellID}].EnduranceCost}) /goto :CantCast
                     } else {
                        /if (!${Spell[${tSpellID}].CanMGB}) /goto :CantCast
                        /if (${Me.CurrentEndurance}<${Math.Calc[${Spell[${tSpellID}].EnduranceCost}*2]}) /goto :CantCast
                     }
                     /varset tGem 50
                     /varcalc tCT ${Spell[${tSpellID}].MyCastTime}/100+20.99
                  }
                  /if (!${tGem}) /goto :CantCast
                  /varset tSpellRange ${Spell[${tSpellID}].Range}
                  /varset tTargetID 0
                  /if (${CCMsgTxt.Arg[3].Length}) {
                     /if (${CCMsgTxt.Arg[3].Find[%t]} && ${NetBots[${CCMsgFrom}].TargetID}) {
                        /varset tTargetID ${NetBots[${CCMsgFrom}].TargetID}
                     } else /if (${CCMsgTxt.Arg[3].Find[self]}) {
                        /varset tTargetID ${Me.ID}
                     } else /if (${NetBots[${CCMsgTxt.Arg[3]}].ID}) {
                        /varset tTargetID ${Spawn[id ${NetBots[${CCMsgTxt.Arg[3]}].ID}].ID}
                     } else /if (${CCMsgTxt.Arg[4].Equal[warder]} && ${Spawn[${CCMsgTxt.Arg[3].Left[-2]}].Pet.Name.Find[Warder]}) {
                        /varset tTargetID ${Spawn[pc ${CCMsgTxt.Arg[3].Left[-2]}].Pet.ID}
                     } else /if (${Spawn[${CCMsgTxt.Arg[3]}].ID} && ${CCMsgTxt.Arg[3].Equal[${Spawn[${CCMsgTxt.Arg[3]}].Name}]}) {
                        /varset tTargetID ${Spawn[${CCMsgTxt.Arg[3]}].ID}
                     } else /if (${Spawn[${CCMsgTxt.Arg[3]}].ID} && ${CCMsgTxt.Arg[3].Equal[${Spawn[${CCMsgTxt.Arg[3]}].ID}]}) {
                        /varset tTargetID ${Spawn[${CCMsgTxt.Arg[3]}].ID}
                     } else /if (${CCMsgTxt.Arg[3].Find[pet]} && ${Spawn[pc ${CCMsgFrom}].Pet.ID}) {
                        /varset tTargetID ${Spawn[pc ${CCMsgFrom}].Pet.ID}
                     }
                  }
                  /if (!${tTargetID}) {
                     /varset tTargetID ${If[${Spell[${tSpellID}].SpellType.Equal[detrimental]},${NetBots[${CCMsgFrom}].TargetID},${If[${NetBots[${CCMsgFrom}].ID},${NetBots[${CCMsgFrom}].ID},${Spawn[${CCMsgFrom}].ID}]}]}
                     /if (!${tTargetID}) {
                        /if (!${NetBots[${CCMsgFrom}].ID} && (${Spell[${tSpellID}].SpellType.Equal[detrimental]} || ${CCMsgTxt.Arg[3].Find[%t]})) {
                           /target clear
                           /assist ${CCMsgFrom}
                           /delay 2s ${Target.ID}
                           /varset tTargetID ${Target.ID}
                        }
                     }
                  }
:tPrepBuff
                  /if (${Select[${CCMsgTxt.Arg[3]},netbots,all]}) {
                     /varcalc tNBMember ${tNBMember}+1
                     /if (${tNBMember}<=${NetBots.Counts}) {
                        /varset tTargetID ${NetBots[${NetBots.Client[${tNBMember}]}].ID}
                        /if (!${Spawn[${tTargetID}].ID} || ${Spawn[${tTargetID}].Distance}>${Spell[${tSpellID}].MyRange}) {
                           /bc Msg: Cast canceled for ${Spawn[${tTargetID}].Name}. Not in range!
                           /goto :tPrepBuff
                        }
                     }
                  } else /if (${CCMsgTxt.Arg[3].Equal[grp]}) {
                     /if (${Group.Member[${tGrpMbr}].ID}) {
                        /varset tTargetID ${Group.Member[${tGrpMbr}].ID}
                        /if (!${Spawn[${tTargetID}].ID} || ${Spawn[${tTargetID}].Distance}>${Spell[${tSpellID}].MyRange}) {
                           /bc Msg: Cast canceled for ${Spawn[${tTargetID}].Name}. Not in range!
                           /goto :tPrepBuff
                        }
                     } else {
                        /varset tTargetID ${Me.ID}
                     }
                  }
                  /if (${tMGB} || ${Select[${Spell[${tSpellID}].Name},Gate,Mass Group Buff]} || (${Spell[${tSpellID}].TargetType.Equal[self]} && !${Me.AltAbility[Project Illusion]})) {
                     /varset tTargetID ${Me.ID}
                  } else /if (${Spell[${tSpellID}].TargetType.Equal[self]} && ${Me.AltAbility[Project Illusion]} && (${Spawn[${tTargetID}].ID} && ${tTargetID}!=${Me.ID} || ${CCMsgTxt.Arg[3].Find[%t]} || ${Spawn[${CCMsgTxt.Arg[3]}].ID})) {
                     /varset ProjectIllu TRUE
                     /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Projecting
                  }
                  /if (${Spell[${tSpellID}].TargetType.Equal[pet]}) /varset tTargetID ${Me.Pet.ID}
                  /if (!${tTargetID}) /goto :CantCast
                  /if (${Spell[${tSpellID}].TargetType.Find[single]}) {
                     /if (${Spawn[${tTargetID}].Distance3D}>${tSpellRange}) {
                        /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Out of Range
                        /goto :CantCast
                     }
                  }
                  /if (${tGem}==50) {
                     /varset CCtTargetID ${tTargetID}
                     /if (${Target.ID}!=${CCtTargetID} && ${CCtTargetID} && (${tTargetID}!=${Me.ID} && ${Spell[${tSpellID}].TargetType.NotEqual[self]})) {
                        /squelch /tar id ${CCtTargetID}
                        /delay 1s ${Target.ID}==${CCtTargetID}
                     }
                     /if (${tMGB}) {
                        /casting "${tMGBName}${PipeChar}alt" -maxtries${PipeChar}3
                        /delay 1s !${Me.AltAbilityReady[${tMGBName}]}
                        /delay 5
                     }
                     /disc ${Spell[${tSpellID}].Name}
                     /goto :CastDone
                  }
                  /varset CCtTargetID 0
                  /varset tCastTime ${tCT}
                  /call ResetOutDoors
:tCasting
                  /if (${CCtTargetID}) {
                     /doevents ImHit
                     /if (${APAggro}) /goto :tContCast
                     /if (${Me.Class.ShortName.Equal[BRD]}) {
                        /if (!${CastResult.Equal[PENDING]}) {
                           /varset ADEventSpell ${Me.Gem[${tGem}]}
                           /call CheckSpellResists
                           /if (${ADResistMatch.Length}) {
                              /if (${ADResistMatch.Equal[STUNNED]}) {
                                 /varset CastResult ${ADResistMatch}
                              } else /if (${ADResistMatch.Find[RESIST]}) {
                                 /if (${ADResistMatch.Find[IMMUNE]}) {
                                    /varset CastResult IMMUNE
                                 } else {
                                    /varset CastResult RESIST
                                 }
                              }
                           }
                           /if (${Select[${CastResult},SUCCESS,IMMUNE,RESIST]}) /goto :tContCast
                           /if (${CastResult.Equal[STUNNED]}) {
                              /varset CCtTargetID 0
                              /goto :tCasting
                           }
                        }
                        /delay 2
| We either finished casting it, or are continually casting it while paused (like Selos for travelling)
                        /if (${Me.Casting.ID}!=${Spell[${Me.Gem[${tGem}]}].ID} || (${MBPause} && ${Twist.List.Equal[${tGem} ]})) {
                           /varset CastResult SUCCESS
                           /delay 1s
                        }
                        /goto :tCasting
                     }
                     /if (${Cast.Timing} && !${Cast.Status.Equal[i]}) /goto :tCasting
                     /if (${Cast.Status.Equal[i]}) {
                        /if (${Select[${Cast.Result},cast_success,cast_resist]}) /goto :tContCast
                        /if (${Select[${Cast.Result},cast_fizzle,cast_recover]}) {
                           /varset tCastTime ${tCT}
                           /varset CCtTargetID 0
                           /goto :tCasting
                        }
                        /if (${Select[${Cast.Result},cast_immune,cast_takehold]}) {
                           /bc Msg: Target Immune or wont Take Hold
                           /goto :tContCast
                        }
| Everything else - CAST_OUTOFRANGE,CAST_CANNOTSEE,CAST_DISTRACTED,CAST_NOTARGET,CAST_INTERRUPTED,CAST_CANCELLED,CAST_NOTREADY,CAST_OUTOFMANA (and anything else we missed)
                        /goto :CantCast
                     }
                  } else /if (${Me.Class.ShortName.Equal[BRD]} || (${Cast.Ready[${tSpell}${PipeChar}${sGem}]} || (${Cast.Status.Equal[i]} && ${tGem}<19 && !${Me.GemTimer[${tGem}]})) && !${Spell[tSpellID].TargetType.Equal[self]}) {
                     /varset CCtTargetID ${tTargetID}
                     /if (${Target.ID}!=${CCtTargetID} && ${CCtTargetID} && (${tTargetID}!=${Me.ID} || ${Spell[${tSpellID}].TargetType.NotEqual[self]})) {
                        /varset tPrevTarget ${Target.ID}
|                       /if (${Me.Class.ShortName.Equal[BRD]}) /squelch /stoptwist
                        /if (${Melee.Combat}) /attack off
                        /squelch /tar id ${CCtTargetID}
                        /delay 1s ${Target.ID}==${CCtTargetID}
                     } else {
                        /varset tPrevTarget 0
                     }
                     /if (${Me.Class.ShortName.Equal[BRD]}) {
                        /if (${Me.State.Equal[sit]}) /stand
                        /if (${tGem}==30) {
                           /if (${${tCastClass.Arg[${a}]}BardSlot[${b}]}) {
                              /stopsong
                              /squelch /twist once ${${tCastClass.Arg[${a}]}BardSlot[${b}]}
                           } else {
                              /alt act ${Me.AltAbility[${tSpell}].ID}
                           }
                           /delay 2
                           /if (${tPrevTarget}) /target id ${tPrevTarget}
                           /goto :tContCast
                        } else /if (${tGem}==20) {
                           /stopsong
|                          /nomodkey /useitem "${tSpell}"
                           /squelch /twist once ${${tCastClass.Arg[${a}]}BardSlot[${b}]}
                           /delay 2
                           /if (${tPrevTarget}) /target id ${tPrevTarget}
                           /delay ${Math.Calc[${tCT}-50]}
                           /goto :tContCast
                        } else {
                           /call ResetSpellResists
                           /varset CastResult PENDING
| If paused, continue twisting this song (like Selos for traveling, etc) until unpaused again. Otherwise, just twist this song once.
                           /if (${MBPause}) {
                              /squelch /stoptwist
                              /squelch /twist ${tGem}
                           } else {
                              /squelch /twist once ${tGem}
                           }
                           /delay 5
                           /if (${tPrevTarget}) /target id ${tPrevTarget}
                           /goto :tCasting
                        }
                     } else {
                        /if (${ProjectIllu}) {
                           /casting "Project Illusion${PipeChar}alt" -targetid${PipeChar}${tTargetID}
                           /delay 1s
                        }
                        /if (${tMGB}) {
                           /casting "${tMGBName}${PipeChar}alt" -maxtries${PipeChar}3
                           /delay 1s !${Me.AltAbilityReady[${tMGBName}]}
                           /delay 5
                        }
                        /varset tCastTime ${tCT}
                        /varset MBCurrentSpell CC${PipeChar}1${PipeChar}${tTargetID}
                        /if (${CCMsgTxt.Arg[4].Equal[CastCheck]}) {
                           /if (${tGem}>19) {
                              /call MQ2Cast "${tSpell}" ${sGem} ${tCastTime} CastCheck -targetid${PipeChar}${tTargetID} ${If[${tFoci.Length},-bandolier${PipeChar}${tFoci},]}
                           } else {
                              /call MQ2Cast "${Spell[${tSpellID}]}" gem${tGem} ${tCastTime} CastCheck -targetid${PipeChar}${tTargetID} ${If[${tFoci.Length},-bandolier${PipeChar}${tFoci},]}
                           }
                           /if (${interruptFlag}) /goto :CantCast
                        } else {  
                           /if (${tGem}>19) {
                              /call MQ2Cast "${tSpell}" ${sGem} ${tCastTime} -targetid${PipeChar}${tTargetID} ${If[${tFoci.Length},-bandolier${PipeChar}${tFoci},]}
                           } else {
                              /call MQ2Cast "${Spell[${tSpellID}]}" gem${tGem} ${tCastTime} -targetid${PipeChar}${tTargetID} ${If[${tFoci.Length},-bandolier${PipeChar}${tFoci},]}
                           }
                        }
                        /if (${Macro.Return.Equal[cast_success]}) /goto :tContCast
                     }
                  }
                  /if (${tCastTime} || ${Cast.Status.NotEqual[i]}) /goto :tCasting
                  /if (${MBDebug}) {
                     /echo ${If[${TimeStampOnEcho},[${Time}] ,]}CAST_not_i - ${Time} - ${tSpell} - ${Spell[${tSpellID}]} - ${MBCurrentSpell} - ${interruptFlag} - ${Cast.Status} - ${Cast.Result} - ${Cast.Stored} - ${NetBots[${TankName.Arg[1,${PipeChar}]}].TargetID} - ${tTargetID} - ${tSpellID} - ${tCT} - ${tCastTime}
                  }
                  /goto :CantCast
:tContCast
                  /doevents OUTDOORS
                  /if (${Cast.Result.Equal[CAST_OUTDOORS]} || ${OutDoorsFail} && ${tSpellID}!=3364) {
                     /call AddToRestrictedList ${Spell[${tSpellID}].ID}
                     /call BuildRestrictedList
                     /goto :CantCast
                  }
                  /varset sReturn ${Cast.Result} ${sReturn} ${tSpellID} ${CCtTargetID}
                  /if (${Me.Class.ShortName.Equal[BRD]} && !${MBPause}) {
                     /if (${tTwistGems.Length}) {
                        /if (${Twist.List.NotEqual[${tTwistGems}]}) {
                           /squelch /twist ${tTwistGems}
                        }
                     } else /if (${Twist.Twisting}) {
                        /squelch /twist off
                     }
                  } 
                  /if (!${Select[${Cast.Result},cast_immune,cast_takehold]} && !${Select[${tCCMsgArg1},qcast,scast]}) {
                     /if (${tSpell.Find[from list]}) {
                        /bc Cast: -[ Bard ]- ${tSpell}
                     } else {
                        /bc Cast: -[ ${Spawn[${CCtTargetID}].CleanName} ]- with ${tSpell}
                     }
                  }
                  /if (${Cursor.ID}) /call ClearCursor
                  /doevents eqbc
                  /if (${Select[${CCMsgTxt.Arg[3]},netbots,all]}) {
                     /varcalc tNBMember ${tNBMember}+1
                     /if (${tNBMember}<=${NetBots.Counts}) {
                        /varcalc tCT ${tCT}+40
                        /goto :tPrepBuff
                     }
                  } else /if (${CCMsgTxt.Arg[3].Equal[grp]}) {
                     /varcalc tGrpMbr ${tGrpMbr}+1
                     /if (${Group.Member[${tGrpMbr}].ID}) {
                        /varcalc tCT ${tCT}+40
                        /goto :tPrepBuff
                     }
                  }
                  /varcalc CastCount ${CastCount}+1
                  /if (${CCMsgTxt.Arg[1].Equal[coresist]}) /goto :CastDone
               }
            /next c
         /next b
      }
   /next a
   
   /if (!${CastCount}) {
      /if (${NetBots[${CCMsgFrom}].ID}) {
         /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Couldn't find Alias [${CCMsgTxt.Arg[2, ]}]
      }
      /return NOALIASFND
   }   
   
   /if (${CCMsgTxt.Arg[3].Equal[on]} || ${CCMsgTxt.Arg[3].Equal[off]}) /return SUCCESS

   /if (!${tSCast}) {
      /varset d ${If[${CCMsgTxt.Arg[3].Equal[%t]} || ${CCMsgTxt.Arg[3].Equal[grp]} || ${NetBots[${CCMsgTxt.Arg[3]}].ID} || ${CCMsgTxt.Arg[3].Equal[self]} || ${CCMsgTxt.Arg[3].Equal[${Spawn[${CCMsgTxt.Arg[3]}].ID}]},4,3]}
      /varset tCCMsgTxt
      /while (${CCMsgTxt.Arg[${d}].Length}) {
         /if (${CCMsgTxt.Arg[${Math.Calc[${d}+1]}].Equal[warder]}) {
            /varcalc d ${d} + 2
         }
         /if (${CCMsgTxt.Arg[${d}].Length}) {
            /varset tCCMsgTxt ${tCCMsgTxt} ${CCMsgTxt.Arg[${d}]}
         }
         /varcalc d ${d}+1
         /delay 1
      }
      /if (${tCCMsgTxt.Length}) {
         /varset CCMsgTxt ${tCCMsgArg1} ${tCCMsgTxt}
         /goto :AliasLoop
      }
   }
:CastDone
   /if (!${Me.ID}) {
      /call Event_DelayOnZone
   }
   /varset MBCurrentSpell
   /if (${Me.Class.ShortName.Equal[BRD]} && !${MBPause}) {
      /if (${Twist.List.NotEqual[${tTwistGems}]} && ${tTwistGems.Length}) /squelch /twist ${tTwistGems}
   }
   /return ${sReturn}

:CantCast
   /if (${Me.Class.ShortName.Equal[BRD]} && !${MBPause}) {
      /if (${Twist.List.NotEqual[${tTwistGems}]} && ${tTwistGems.Length}) /squelch /twist ${tTwistGems}
   }
   /if (${Spell[${tSpellID}].SpellType.Equal[detrimental]}) {
      /if (${CCtTargetID}) {
         /if (${Spawn[${CCtTargetID}].Type.Equal[corpse]} || !${Spawn[${CCtTargetID}].ID}) /return FAIL ${sReturn}
      } else /if (${tTargetID}) {
         /if (${Spawn[${tTargetID}].Type.Equal[corpse]} || !${Spawn[${tTargetID}].ID}) /return FAIL ${sReturn}
      }
      /if (${Cast.Result.Equal[cast_notarget]} && ${Spell[${tSpellID}].TargetType.Equal[target of target]}) /return FAIL ${sReturn}
   }
   /if (!${interruptFlag}) /bc Msg: There was a problem and I can't cast ${tSpell} right now
/return FAIL ${sReturn}


Sub CheckMGB(string tCommand)
   /if (!${AllowMGB}) /return FALSE
   /declare tMGBName string local
   /if (${Me.AltAbilityReady[Tranquil Blessings]} && ${Select[${Me.CombatState},ACTIVE,RESTING]}) {
      /varset tMGBName Tranquil Blessings
   } else /if (!${Select[${tCommand},tmgb,tbomb]} && (!${Me.AltAbility[Tranquil Blessings]} || ${AllowMGB}>1)) {
      /if (${Me.AltAbilityReady[Mass Group Buff]}) {
         /varset tMGBName Mass Group Buff
      } else /if (${Me.AltAbilityReady[Cry of Battle]}) {
         /varset tMGBName Cry of Battle
      }
   }
/return ${tMGBName}


Sub ProcessExcludeList(P0)
   /declare a int local
   /declare sID string local
   /if ((${P0.Length}) || !${Zone.Name.Equal[${ZoneName}]}) {
      /if (${P0.Equal[save]}) {
         /ini "${MBCommon}" "ExcludeList" "${Zone.ShortName}" "${ExcludeList}"
         /return
      } else /if (${P0.Equal[zone]} || !${Zone.Name.Equal[${ZoneName}]}) {
         /varset ExcludeList ${If[${Select[${Ini["${MBCommon}","ExcludeList","${Zone.ShortName}",NOTFOUND]},NOTFOUND,NULL]},${PipeChar},${Ini["${MBCommon}","ExcludeList","${Zone.ShortName}",NOTFOUND]}]}
         /varset ExcludeListID ${PipeChar}
         /for a 1 to ${NetBots.Counts}
            /if (${NetBots.Client.Arg[${a}].NotEqual[${Me.CleanName}]} && ${NetBots[${NetBots.Client.Arg[${a}]}].InZone}) {
               /if (${NetBots[${NetBots.Client.Arg[${a}]}].PetID} && !${Spawn[${NetBots[${NetBots.Client.Arg[${a}]}].PetID}].IsSummoned}) /varset ExcludeListID ${ExcludeListID}${NetBots[${NetBots.Client.Arg[${a}]}].PetID}${PipeChar}
            }
         /next a
      }
   }
   /if (!${ExcludeList.Left[1].Equal[${PipeChar}]}) /varset ExcludeList ${PipeChar}${ExcludeList}
   /if (!${ExcludeListID.Left[1].Equal[${PipeChar}]}) /varset ExcludeListID ${PipeChar}${ExcludeListID}
|  /echo ${If[${TimeStampOnEcho},[${Time}] ,]}${ExcludeList} 
   /squelch /alert clear ${AlertList}
   /squelch /alert add ${AlertList} NPC "eye of "
   /if (${ExcludeList.Length}>1) {
      /for a 1 to ${Math.Calc[${ExcludeList.Count[${PipeChar}]}-1]}
         /if (${ExcludeList.Arg[${a},${PipeChar}].Find[#]}==1) {
            /squelch /alert add ${AlertList} NPC "${ExcludeList.Arg[${a},${PipeChar}].Right[-1]}"
         } else {
            /squelch /alert add ${AlertList} NPC "${ExcludeList.Arg[${a},${PipeChar}]}"
         }
      /next a
   }
   /if (${ExcludeListID.Length}>1) {
      /for a 1 to ${Math.Calc[${ExcludeListID.Count[${PipeChar}]}-1]}
         /squelch /alert add ${AlertList} id "${ExcludeListID.Arg[${a},${PipeChar}]}"
      /next a
   }
/return


Sub Login(string name,..)
|  /echo ${If[${TimeStampOnEcho},[${Time}] ,]}${name.Arg[2, ]}
|  /echo ${If[${TimeStampOnEcho},[${Time}] ,]}${MacToRun}
   /if (!${name.Length}) {
      /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Syntax error
      /return
   }
   /declare toon int local
   /if (${Defined[MacToRun]}) /deletevar MacToRun
   /if (${name.Find[${PipeChar}]}) {
      /declare MacToRun string global ${name.Arg[2,${PipeChar}]}
   }
   /bc Msg: Logging into ${name.Arg[1,${PipeChar}]}
   /camp
   /delay 10m ${MacroQuest.GameState.Equal[CHARSELECT]}
   /varset toon ${Window[CharacterListWnd].Child[CLW_LeftScreen].Child[CLW_CharactersScreen].Child[CLW_Character_List].List[${name.Arg[1,${PipeChar}]},3]}
   /nomodkey /notify CharacterListWnd CLW_Character_List listselect ${toon}
   /nomodkey /notify CharacterListWnd CLW_Character_List leftmouse ${toon}
   /nomodkey /notify CharacterListWnd CLW_Play_Button leftmouseup
   /delay 10m ${MacroQuest.GameState.Equal[INGAME]}
|  /delay 3m ${Me.ID}
   /if (${Zone.ShortName.Equal[Bazaar]} && ${Me.Name.Equal[${TraderName.Arg[1,${PipeChar}]}]} && !${Defined[MacToRun]}) {
      /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Trader logged in.
      /if (!${Window[BazaarWnd]}) /timed 10 /trader
      /timed 30 /if (${Window[BazaarWnd].Child[BZW_Start_Button]}) /nomodkey /notify BazaarWnd BZW_Start_Button leftmouseup
      /endmacro
   } else /if (${Zone.ShortName.Equal[Bazaar]} && ${Me.Name.Equal[${TraderName.Arg[1,${PipeChar}]}]} && ${Defined[MacToRun]}) {
      /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Trader logged in.  Starting ${MacToRun} macro.
      /timed 100 /mac ${MacToRun}
      /timed 150 /deletevar MacToRun
      /endmacro
   } else {
      /mqp
      /timed 100 /macro modbot
   }
/return


Sub Event_ResetMelee
   /melee reset
/return


Sub Event_WornOff(Line,SpellName,Toon)
   /if (!${Spell[${ADCharmSpell}].ID} && !${ABHasRecast}) {
      /doevents flush WornOff
      /return
   }
   /if (${SpellName.Equal[${ADCharmSpell}]}) {
      /call CharmBroke
      /return
   }
   /if (${NetBots[${Toon}].ID}) /return
   /if (!${ABHasRecast}) /return
   /declare i int local
   /declare spwnType string local
   /if ((${Toon.Find['s warder]} || ${Toon.Find['s Pet]}) && ${Spawn[Pet ${Toon}].ID}) {
      /varset spwnType Pet
   } else {
      /varset spwnType ${Spawn[${Toon}].Type}
   }
   /if (${Select[${Spawn[${Toon}].Type},PC,Pet]}) {
      /for i 1 to ${ABCount}
         /if (${ABRecast[${i}]}) {
            /if (${SpellName.Equal[${Spell[${ABSpell[${i}]}].Name}]}) {
               /call AddToQueue ${Spawn[${spwnType} ${Toon}].ID} ${i}
            }
         }
      /next i
   }
/return


Sub DoBuffEvents
   /if (${MBDebug}) {
      /echo ${If[${TimeStampOnEcho},[${Time}] ,]}BE - ${Macro.RunTime} - ${MBCurrentSpell} - ${interruptFlag} - ${Cast.Status} - ${Cast.Result} - ${Cast.Stored} - ${NetBots[${TankName.Arg[1,${PipeChar}]}].TargetID} - ${Target.ID} - ${Me.Casting.ID} - ${QueueCount}
   }
   /if (${MBPause}) /return
   /declare a int local
   /declare TempVar int local ${Math.Calc[${ABCount}+1]}
   /declare TempGuy int local 0
   /for a 1 to 25
      /if (${DoBuffQueue[${a},2]}<${TempVar} && ${Spawn[${DoBuffQueue[${a},1]}].ID} && ${Spawn[${DoBuffQueue[${a},1]}].Type.NotEqual[corpse]} && (${Spawn[${DoBuffQueue[${a},1]}].Distance}<${Me.Book[${Me.Book[${ABSpell[${DoBuffQueue[${a},2]}]}]}].MyRange} || ${Spawn[${DoBuffQueue[${a},1]}].Distance}<${Me.Book[${Me.Book[${ABSpell[${DoBuffQueue[${a},2]}]}]}].AERange})) {
         /varset TempGuy ${DoBuffQueue[${a},1]}
         /varset TempVar ${DoBuffQueue[${a},2]}
      }
|     /if (!${Spawn[${DoBuffQueue[${a},1]}].ID}) /call RemoveFromQueue ${DoBuffQueue[${a},1]} ${DoBuffQueue[${a},2]}
   /next a
   /if (!${ACState} && ${DoMelee} && ${ADMobFound}) /return
   /if (!${SpawnCount[npc radius ${Radius} zradius ${ZRadius} noalert ${AlertList}]} || ${ABTarType[${TempVar}].Find[cbt]} || ${Select[${ABGem[${TempVar}]},item,alt]} || ${Me.Gem[${ABSpell[${TempVar}]}]}) {
      /if (${TempGuy}) /if (((${Me.SpellReady[${ABSpell[${TempVar}]}]} || ${Spell[${ABSpell[${TempVar}]}].RecastTime.TotalSeconds}<30) && ${Me.CurrentMana}>${Spell[${ABSpell[${TempVar}]}].Mana} && ${Me.PctMana}>=${ABSpellMinMana[${TempVar}]}) || (${ABGem[${TempVar}].Equal[item]} && ${FindItem[${ABSpell[${TempVar}]}].TimerReady}<30) || (${ABGem[${TempVar}].Equal[alt]} && ${Me.AltAbilityReady[${ABSpell[${TempVar}]}]}<30)) {
         /call MQ2Cast "${ABSpell[${TempVar}]}" ${If[!${Select[${ABGem[${TempVar}]},item,alt]},gem,]}${ABGem[${TempVar}]} 10s CastCheck -targetid${PipeChar}${TempGuy}
         /if (${castReturn.Equal[CAST_TAKEHOLD]} || ${castReturn.Equal[CAST_SUCCESS]} || ${Cast.Return.Equal[CAST_OUTDOORS]}) {
            /call RemoveFromQueue ${TempGuy} ${TempVar}
         }
      }
   }
/return


Sub AddToQueue(int Buffee,int BuffNumber)
   /if (${MBDebug}) {
      /echo ${If[${TimeStampOnEcho},[${Time}] ,]}AQ - ${Macro.RunTime} - ${MBCurrentSpell} - ${interruptFlag} - ${Cast.Status} - ${Cast.Result} - ${Cast.Stored} - ${NetBots[${TankName.Arg[1,${PipeChar}]}].TargetID} - ${Target.ID} - ${Me.Casting.ID} - ${Buffee} - ${BuffNumber}
   }
   /declare a int local
|  /if (!${RestrictedList.Find[${PipeChar}${DoBuffID[${BuffNumber}]}${PipeChar}]} && ${QueueCount}<25 && ${Spawn[${Buffee}].Type.NotEqual[corpse]} && ${Spawn[${Buffee}].ID}) {
   /if (${QueueCount}<25 && ${Spawn[${Buffee}].Type.NotEqual[corpse]} && ${Spawn[${Buffee}].ID}) {
      /for a 1 to 25
         /if (!${DoBuffQueue[${a},1]}) {
            /varset DoBuffQueue[${a},1] ${Buffee}
            /varset DoBuffQueue[${a},2] ${BuffNumber}
            /varcalc QueueCount ${QueueCount}+1
            /return
         }
      /next a
   }
/return


Sub RemoveFromQueue(int Buffee,int BuffNumber)
   /if (${MBDebug}) {
      /echo ${If[${TimeStampOnEcho},[${Time}] ,]}RQ - ${Macro.RunTime} - ${MBCurrentSpell} - ${interruptFlag} - ${Cast.Status} - ${Cast.Result} - ${Cast.Stored} - ${NetBots[${TankName.Arg[1,${PipeChar}]}].TargetID} - ${Target.ID} - ${Me.Casting.ID} - ${Buffee} - ${BuffNumber}
   }
   /declare a int local
   /declare tABMob int local
   /for a 1 to 25
      /if (((${Spawn[${DoBuffQueue[${a},1]}].State.Equal["DEAD"]} || !${Spawn[${DoBuffQueue[${a},1]}].ID}) || ((${DoBuffQueue[${a},1]}==${Buffee} || (!${Buffee} && (${Group.Member[${Spawn[${DoBuffQueue[${a},1]}].CleanName}].Index} || ${Group.Member[${Spawn[${DoBuffQueue[${a},1]}].Master.CleanName}].Index}))) && ${DoBuffQueue[${a},2]}==${BuffNumber})) && ${DoBuffQueue[${a},1]}) {
         /varset DoBuffQueue[${a},1] 0
         /varset DoBuffQueue[${a},2] 0
         /varcalc QueueCount ${QueueCount}-1
      }
   /next a
/return


Sub TellEvent(string From, string MsgText)
   /if (!${DoTells}) /return
   /bc Msg: ${Time} [+g+]Tell Received from[+x+] ${From} : ${MsgText}
   /docommand /${BeepCommand}
/return


Sub Event_Camping
   /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Campout detected. Ending macro.
   /if (${Cursor.ID}) /call ClearCursor force
   /afollow off
   /call MainExit
   /endmacro
/return


Sub CastCheck(timer giveUpTimer)
| Let's do the looping here, instead of repeatedly calling it
   /while (${Cast.Status.Find[C]}) {   
      /if (!${giveUpTimer}) /return
      /if (${DoHeals} && ${AHCount} && !${MBCurrentSpell.Arg[4,${PipeChar}].Equal[NHI]}) /call AHHealCheck
      /if (${Cast.Status.Equal[I]} || ${interruptFlag}) /return
      /if (${Window[RespawnWnd].Open} || ${NetBots[${Me.CleanName}].Charmed}) {
         /varset interruptFlag TRUE
         /return
      }
      /if ((${Target.Type.Equal[corpse]} || !${Target.ID}) && ${Me.Casting.ID} && ${Spell[${Me.Casting.ID}].TargetType.Find[Single]}) {
         /call interrupt
         /return
      }
      /if (${MBCurrentSpell.Arg[1,${PipeChar}].Equal[AD]} && !${ACState}) {
         /if (${Me.Casting.ID} && !${Spell[${Me.Casting.ID}].TargetType.Find[ ae]} && !${Spell[${Me.Casting.ID}].SpellType.Find[Beneficial]}) {
            /if (${Select[${ADTarType[${MBCurrentSpell.Arg[2,${PipeChar}]}]},1,11,3,13]}) {
               /if (${NetBots[${TankName.Arg[1,${PipeChar}]}].ID}) {
                  /if (${NetBots[${TankName.Arg[1,${PipeChar}]}].TargetID}!=${MBCurrentSpell.Arg[3,${PipeChar}]}) {
                     /call interrupt
                     /return
                  }
               } else {
                  /if (${ACMATarget}!=${MBCurrentSpell.Arg[3,${PipeChar}]}) {
                     /call interrupt
                     /return
                  }
               }
            } else /if (${Select[${ADTarType[${MBCurrentSpell.Arg[2,${PipeChar}]}]},2,12]}) {
               /if (${NetBots[${TankName.Arg[1,${PipeChar}]}].ID}) {
                  /if (${NetBots[${TankName.Arg[1,${PipeChar}]}].TargetID}==${MBCurrentSpell.Arg[3,${PipeChar}]}) {
                     /call interrupt
                     /return
                  }
               } else {
                  /if (${ACMATarget}==${MBCurrentSpell.Arg[3,${PipeChar}]}) {
                     /call interrupt
                     /return
                  }
               }
            }
         }
      } else /if (${Select[${MBCurrentSpell.Arg[1,${PipeChar}]},AB,AE]} && ${Me.Casting.ID}) {
| If we are following, don't let the toon fall behind due to buffing.
         /if (${MBCurrentSpell.Arg[1,${PipeChar}].Equal[AB]} && ${FollowState}) {
            /if (${Spawn[${FollowID}].Distance}>${FollowDistanceCheck}) {
               /call interrupt
               /return
            }
         }
         /if (${${MBCurrentSpell.Arg[1,${PipeChar}]}TarType[${MBCurrentSpell.Arg[2,${PipeChar}]}].Find[cbt]}) {
            /if (!${${MBCurrentSpell.Arg[1,${PipeChar}]}TarType[${MBCurrentSpell.Arg[2,${PipeChar}]}].Find[idle]}) {
               /call ADSpawnCheck Find
               /if (!${ADMobFound}) {
                  /call interrupt
                  /return
               }
            }
         } else /if (${Cast.Timing}>1000 || ${Cast.Status.Find[M]}) {
            /call ADSpawnCheck Find
            /if (${ADMobFound}) {
               /call interrupt
               /return
            }
         }
      }
   }
/return


Sub ParseAnnounce(string sMsg, string sText, string sTarget, string sSpell)
   /if (${sText.Length}>10) {
      /if (${sText.Find[%t]}) {
         /varset sText ${sText.Replace[%t,${sTarget}]}
      }
      /if (${sText.Find[%s]}) {
         /varset sText ${sText.Replace[%s,${sSpell}]}
      }
   } else {
      /varset sText ${sText} ${sMsg} -[ ${sTarget} ]- with ${sSpell}
   }
   /docommand ${sText}
/return


Sub IniCommand(string IniText)
| /echo ${If[${TimeStampOnEcho},[${Time}] ,]}IniCommand: Text = ${IniText}
  /if (${IniText.Length}<3) /return
  /declare b int local 1
  /declare sVarC string local
  /declare sVarL string local
  /declare sVarM string local
  /for b 1 to ${IniText.Count[{]}
    /varset sVarL ${IniText.Arg[${b},{]}
    /varset sVarC ${sVarC}${sVarL}${
      }
  /next b
  /varset sVarC ${sVarC}${IniText.Arg[${b},{]}
  /if (!${IniText.Count[{]}) /varset sVarC ${IniText}
  /varset sVarM
  /for b 1 to ${sVarC.Count[']}
    /if (${sVarC.Find['']}) {
      /varset sVarL ${sVarC.Mid[1,${Math.Calc[${sVarC.Find['']}-1]}]}"
| "
      /varset sVarC ${sVarC.Mid[${Math.Calc[${sVarC.Find['']}+2]},${sVarC.Length}]}
      /varset sVarM ${sVarM}${sVarL}
    }
  /next b
  /varset sVarM ${sVarM} ${sVarC}
  /if (${sVarM.Length}) /varset sVarC ${sVarM}
| /echo ${If[${TimeStampOnEcho},[${Time}] ,]}"${sVarC}"
  /docommand ${sVarC}
/return FALSE


Sub Event_GoM(string eLine,string eSkip,string eMax)
| /echo ${If[${TimeStampOnEcho},[${Time}] ,]}GOM: ${eLine} - ${eSkip} - ${eMax}
   /if (${MBPause} || !${GoMNuke.Length} || (!${Me.Song[Gift of Mana (${eMax})].ID} && !${Me.Song[Gracious Gift of Mana (${eMax})].ID})) /return
   /declare x int local 0
   /declare IniSpell string local
   /for x 1 to ${Math.Calc[${GoMNuke.Count[${PipeChar}]}+1]}
      /call FindAlias ${GoMNuke.Arg[${x},${PipeChar}]}
      /if (!${Macro.Return.Length} || ${Macro.Return.Equal[NULL]}) /return
      /varset IniSpell ${Macro.Return}
      /if ((${Spell[${${IniSpell.Arg[1,${PipeChar}]}Spell[${IniSpell.Arg[2,${PipeChar}]}]}].SpellType.Equal[detrimental]} && (!${Spawn[${ACMATarget}].ID} || ${Spawn[${ACMATarget}].Type.Equal[corpse]})) || ${Spell[${${IniSpell.Arg[1,${PipeChar}]}Spell[${IniSpell.Arg[2,${PipeChar}]}]}].Level}>${Int[${eMax}]}) {
         /if (${GoMNuke.Find[${PipeChar}]} && ${Math.Calc[${x}-1]}<${GoMNuke.Count[${PipeChar}]}) /continue
         /return
      }
      /delay 2s ${Cast.Ready[${${IniSpell.Arg[1,${PipeChar}]}Spell[${IniSpell.Arg[2,${PipeChar}]}]}${PipeChar}gem${${IniSpell.Arg[1,${PipeChar}]}Gem[${IniSpell.Arg[2,${PipeChar}]}]}]}
      /if (!${Cast.Ready[${${IniSpell.Arg[1,${PipeChar}]}Spell[${IniSpell.Arg[2,${PipeChar}]}]}${PipeChar}gem${${IniSpell.Arg[1,${PipeChar}]}Gem[${IniSpell.Arg[2,${PipeChar}]}]}]} && ${GoMNuke.Find[${PipeChar}]} && ${Math.Calc[${x}-1]}<${GoMNuke.Count[${PipeChar}]}) /continue
      /call MQ2Cast "${${IniSpell.Arg[1,${PipeChar}]}Spell[${IniSpell.Arg[2,${PipeChar}]}]}" gem${${IniSpell.Arg[1,${PipeChar}]}Gem[${IniSpell.Arg[2,${PipeChar}]}]} 10s CastCheck ${If[${Spell[${${IniSpell.Arg[1,${PipeChar}]}Spell[${IniSpell.Arg[2,${PipeChar}]}]}].SpellType.Equal[detrimental]},-targetid${PipeChar}${ACMATarget},-targetid${PipeChar}${Spawn[${TankID}].ID}]}
      /echo ${If[${TimeStampOnEcho},[${Time}] ,]}GoMNuke : ${${IniSpell.Arg[1,${PipeChar}]}Spell[${IniSpell.Arg[2,${PipeChar}]}]}
      /return
   /next x
/return


Sub FindAlias(SpellAlias)
   /declare tCastClass string local AH AD AB AE AQ
   /declare d int local
   /declare x int local
   /declare y int local
   /declare tSpell string local
   /for d 1 to ${tCastClass.Count[ ]}
      /if (${${tCastClass.Arg[${d}]}Count}) {
          /for x 1 to ${${tCastClass.Arg[${d}]}Count}
             /for y 1 to ${${tCastClass.Arg[${d}]}SpellAlias[${x}].Count[${PipeChar}]}
                /if (${${tCastClass.Arg[${d}]}SpellAlias[${x}].Arg[${y},${PipeChar}].Equal[${SpellAlias}]}) {
                   /varset tSpell ${tSpell}${tCastClass.Arg[${d}]}${PipeChar}${x}${PipeChar}
                }
             /next y
          /next x
      }
   /next d
|  /echo ${If[${TimeStampOnEcho},[${Time}] ,]}tSpell - ${tSpell}
|  AHSpell5 would return as AH|5|
/return ${tSpell}


Sub Event_OutOfStuff
   /if (${DoCombines}) {
      /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Out of materials. Setting DoCombines FALSE
      /varset DoCombines FALSE
   }
/return


Sub IsNumber(Index)
   /if (!${Index.Length}) /return
   /declare x int local
   /for x 1 to ${Index.Length}
      /if (!${Select[${Index.Mid[${x},1]},0,1,2,3,4,5,6,7,8,9,.]}) {
|        /echo ${If[${TimeStampOnEcho},[${Time}] ,]}IsNumber - FALSE - Index was ${Index}
         /return FALSE
      }
   /next x
/return TRUE


Sub SearchVendor
|  /declare CurMerchant int outer 0 - Need to set this to the ID of merchant window.  Grab name from window.
   /declare ICount int local
   /declare mItem int local 0
   /declare mItemName string local
   /if (!${Window[MerchantWnd].Open}) {
      /varset CurMerchant 0
      /return
   }
   /declare VendorName string local ${Window[MerchantWnd].Child[MW_MerchantName].Text}
:MerchPopulate
   /varset ICount ${Merchant.Items}
   /delay 1s
   /if (${ICount}!=${Merchant.Items}) /goto :MerchPopulate
   /varset CurMerchant ${Spawn[merchant ${Window[MerchantWnd].Child[MW_MerchantName].Text}].ID}
   /for mItem 1 to ${Merchant.Items}
      /varset mItemName ${Merchant.Item[${mItem}].Name}
      /if (${Ini["${LootIni}","${mItemName.Left[1]}","${mItemName}"].Find[Buy]}) {
         /echo ${If[${TimeStampOnEcho},[${Time}] ,]}${mItemName} found on ${VendorName}
         /echo ${If[${TimeStampOnEcho},[${Time}] ,]}${Ini["${LootIni}","${mItemName.Left[1]}","${mItemName}"]}
      }
   /next mItem
/return


| AAPurchasing Revamped - 11/01/2017
| Codebase from AAPurchase.inc - customized, restructured, and integrated into ModBot.

Sub AAInit 
   /declare ListNum string outer 
   /declare a int local 
   /if (!${Ini["${AAIniFile}"].Length} ) {
      /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Creating AA_${Me.Name}.ini file
      /call AADump
   } else /if (!${Ini["${AAIniFile}","${AAIniSection}"].Length} ) {
      /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Creating Section ${AAIniSection} in AA_${Me.Name}.ini file
      /call AADump
   }
   /call LoadINIVar ${AAIniSection} AACount 0 AACount ${AAIniFile} int 
   /if (${AACount}) { 
      /declare AA[${AACount}] string outer 
      /for a 1 to ${AACount} 
         /call LoadINIVar ${AAIniSection} AA${a} " " AA[${a}] ${AAIniFile}
      /next a 
      /if (${DoAA}) /call AAPicker force
   } 
/return 


Sub AAPicker(string aaParm)
   /if (${MBPause} || !${AACount} || ${Me.AAPoints}<=0) /return
| If not Fast-AA purchase, and Confirmation box is open from something else, skip AA checking for the moment so as not to accidentally click it
   /if (!${Window[OptionsWindow].Child[OptionsGeneralPage].Child[OGP_AANoConfirmCheckbox].Checked} && ${Window[ConfirmationDialogBox].Open}) /return
   /varset AACheckTimer 5m

   /declare x int local 
   /declare Cost int local 
   /declare AALevelStr string local 
   /declare curAALevel int local 
   /declare maxAALevel int local 
   /declare maxPurchaseLevel int local 
   /declare nameAA string local 
   /declare AACategory string local 
   /declare maxCounter int local 0 
   /declare myMax int local ${If[${MacroQuest.BuildName.Equal[emu]},${Math.Calc[${Me.AAPoints}+100]},${Math.Calc[${Me.Level}*2]}]}
   /declare AAtoSpend int local
   /declare wantSomething bool local FALSE
   /declare wantExpendable bool local FALSE
   /declare doAnnounce string local ${If[${AAAnnounce.Length},${AAAnnounce} Msg:,${If[${aaParm.Equal[force]},/bc Msg:,]}]} 
   /declare doVerbose bool local ${AAVerbose}
   /declare newAAstr string local
   /declare disAAName string local   

   /varcalc AAtoSpend ${Me.AAPoints}-${AABank}
   /if (${AAtoSpend}<=0) {
      /if (${doAnnounce.Length} && ${doVerbose}) {
         /docommand ${doAnnounce} Unable to purchase AA's at this time. You have no spendable AA.
      }
      /return
   }

   /for x 1 to ${AACount} 
      /if (!${AA[${x}].Arg[1,${PipeChar}].Length} || !${AA[${x}].Arg[2,${PipeChar}].Length} || ${AA[${x}].Arg[2,${PipeChar}].Equal[X]}) /continue
      /varset nameAA ${AA[${x}].Arg[1,${PipeChar}]} 
| Sel ListNum to the page the AA is on. 
      /varset ListNum List${AltAbility[${nameAA}].Type} 
:BuyAdditional
| Clear the Can Purchase button and Filters
      /nomodkey /notify AAWindow ResetFilter leftmouseup
| Check if the skill is available to you. 
      /if (${AltAbility[${nameAA}].ID}) { 
         /varset AALevelStr ${Window[AAWindow].Child[${ListNum}].List[${Window[AAWindow].Child[${ListNum}].List[=${nameAA}]}, 2]} 
         /varset curAALevel ${AALevelStr.Arg[1,"/"]} 
         /varset maxAALevel ${AALevelStr.Arg[2,"/"]}
         /if (${Select[${AA[${x}].Arg[2,${PipeChar}]},M,L]}) {
            /varset maxPurchaseLevel ${maxAALevel}
            /if (${curAALevel}>=${maxPurchaseLevel}) {
               /varset newAAstr ${AA[${x}].Arg[1,${PipeChar}]}${PipeChar}X
               /if (${AA[${x}].Arg[3,${PipeChar}].Length}) {
                  /varset newAAstr ${newAAstr}${PipeChar}${AA[${x}].Arg[3,${PipeChar}]}
               }
               /ini "${AAIniFile}" "${AAIniSection}" AA${x} "${newAAstr}" 
               /continue
            }
         } else { 
            /varset maxPurchaseLevel ${AA[${x}].Arg[2,${PipeChar}]} 
            /if (${maxPurchaseLevel}<=0) /continue
            /if (${maxPurchaseLevel}>${maxAALevel}) {
               /varset maxPurchaseLevel ${maxAALevel}
            }
         }
         /varset AACategory ${Window[AAWindow].Child[${ListNum}].List[${Window[AAWindow].Child[${ListNum}].List[=${nameAA}]}, 5]}
         /if (${AACategory.Equal[Expendable]}) {
            /varset wantExpendable TRUE  
            /varset AACheckTimer 1m
         }
         /if (${curAALevel}<${maxPurchaseLevel}) {
            /varset wantSomething TRUE 
            /varset Cost ${Window[AAWindow].Child[${ListNum}].List[${Window[AAWindow].Child[${ListNum}].List[=${nameAA}]}, 3]} 
| Click the Can Purchase button. 
            /if (!${Window[AAWindow].Child[CanPurchaseFilter].Checked}) { 
               /nomodkey /notify AAWindow CanPurchaseFilter leftmouseup
            }
| If the AA still shows in the list, we should be able to purchase it.
            /if (!${Window[AAWindow].Child[${ListNum}].List[=${nameAA}]}==NULL) {
| Check vs. spendable AA (considering Banked AA).
               /if (${Cost}<=${AAtoSpend}) {
                  /call AASelect "${nameAA}"   
                  /call AAPurchase 
                  /varcalc curAALevel ${curAALevel}+1 
                  /if (${doAnnounce.Length}) {
                     /docommand ${doAnnounce} ${Time} Purchased level ${curAALevel}/${maxAALevel} of ${nameAA} for ${Cost} points.
                  }
                  /delay 1s 
| If I purchased an "Enabled" AA, it is probably now Disabled. If it is now Disabled and zero-cost, re-enable it.
                  /if (${nameAA.Find[Enabled]} && ${curAALevel}<${maxPurchaseLevel}) {
                     /varset disAAName ${nameAA.Replace[Enabled,Disabled]}
                     /if (!${Window[AAWindow].Child[${ListNum}].List[=${disAAName}]}==NULL) {
                        /varset Cost ${Window[AAWindow].Child[${ListNum}].List[${Window[AAWindow].Child[${ListNum}].List[=${disAAName}]}, 3]}
                        /if (${Cost}==0) {
                           /call AASelect "${disAAName}"   
                           /call AAPurchase
                           /varcalc curAALevel ${curAALevel}+1 
                           /if (${doAnnounce.Length}) {
                              /docommand ${doAnnounce} ${Time} Purchased level ${curAALevel}/${maxAALevel} of ${disAAName} for ${Cost} points (to re-enable it).
                           }
                           /delay 1s
                        }
                     }
                  }
| If I still have points after purchasing try to purchase again. 
                  /if (${Me.AAPoints}>0 && ${Me.AAPoints}>${AABank}) {
                     /varcalc AAtoSpend ${Me.AAPoints}-${AABank}
                     /if (${AAtoSpend}>0) {
                        /goto :BuyAdditional
                     }
                  }
                  /return
               } else {
                  /if (${doAnnounce.Length} && ${doVerbose}) {
                     /docommand ${doAnnounce} Unable to purchase ${nameAA} at this time. You have ${AAtoSpend} spendable AA (keeping ${AABank} banked) and need ${Cost} AA.
                  }
               }
| Else, check why we can't purchase it
            } else {
| If we are maxing to level, check level first.
               /if (${AA[${x}].Arg[2,${PipeChar}].Equal[L]}) {
                  /call AALevelCheck "${nameAA}"
                  /if (${Macro.Return.NotEqual[TRUE]}) {
                     /if (${doAnnounce.Length} && ${doVerbose}) {
                        /docommand ${doAnnounce} Unable to purchase ${nameAA} at this time. You are Level ${Me.Level} and it requires Level ${Macro.Return}.
                        /docommand ${doAnnounce} Checking if other AA are available.
                     }
                     /continue
                  }
               }
| If announcing, check for other reasons for the inability to purchase, but only if we are announcing the reasons
               /if (${doAnnounce.Length} && ${doVerbose}) {
| Check if Cost is greater than spendable AA. 
                  /if (${Cost}>${AAtoSpend}) { 
                     /docommand ${doAnnounce} Unable to purchase ${nameAA} at this time. You have ${AAtoSpend} spendable AA and need ${Cost} AA.
                  } else {
| Check Level
                     /call AALevelCheck "${nameAA}"
                     /if (${Macro.Return.NotEqual[TRUE]}) {
                        /docommand ${doAnnounce} Unable to purchase ${nameAA} at this time. You are Level ${Me.Level} and it requires Level ${Macro.Return}.
                     } else {
| Click off the Can Purchase button. 
                        /if (${Window[AAWindow].Child[CanPurchaseFilter].Checked}) {
                           /nomodkey /notify AAWindow CanPurchaseFilter leftmouseup
                        }
| You must not have the expansion or other pre-requisites
                        /if (${Window[AAWindow].Child[AAW_ExpansionFilter].List[AACategory]}) {  
                           /docommand ${doAnnounce} You have ${AAtoSpend} AA to spend and ${nameAA} costs ${Cost} AA. You must not have ${AACategory} or you have not met the prerequisites.
                        } else {
                           /docommand ${doAnnounce} You have ${AAtoSpend} AA to spend and ${nameAA} costs ${Cost} AA. You must not have met the prerequisites.
                        }
                     }
                  }
               }
            }
| Check if we are skipping to the next AA, or stopping
            /if (${AA[${x}].Arg[3,${PipeChar}].Equal[S]}) {
               /if (${doAnnounce.Length} && ${doVerbose}) {
                  /docommand ${doAnnounce} Checking if other AA are available.
               }
               /continue
            }
            /return
| Else, the skill is maxed. 
         } else { 
            /if (${doAnnounce.Length} && ${doVerbose}) {
               /if (${curAALevel}==${maxAALevel} && ${maxPurchaseLevel}==${maxAALevel}) {
                  /docommand ${doAnnounce} ${nameAA} is Maxed
               }
            }
         } 
| Else, that skill is not available. 
      } else { 
         /if (${doAnnounce.Length}) {
            /docommand ${doAnnounce} ${nameAA} is not a valid ${Me.Class} Ability.
         }
      } 
   /next x 
| If all skills are maxed
   /if (!${wantSomething} && !${wantExpendable}) { 
      /if (${DoAA}) {
         /if (${doAnnounce.Length}) {
            /docommand ${doAnnounce} No defined skills or all defined skills maxed. Setting DoAA FALSE.
      }
         /varset DoAA FALSE
      } else {
         /if (${doAnnounce.Length}) {
            /docommand ${doAnnounce} No defined skills or all defined skills maxed.
         }
      }
| Else, if not, wait for next round. 
   } else { 
      /if (${doAnnounce.Length} && ${doVerbose}) {
         /docommand ${doAnnounce} Cycled through all AA and nothing to purchase this round.
      }
   }
| If banked AA is greater than or equal to warning level and less than max level, issue a warning.
   /if (${AAWarning} && ${Me.AAPoints}>=${AAWarning} && ${Me.AAPoints}<${myMax}) {
      /if (${doAnnounce.Length}) {
         /docommand ${doAnnounce} Warning! You are at ${Me.AAPoints} banked AA.
      }
   }
| If banked AA equals max, turn off alt exp (as requested). 
   /if (${Me.AAPoints}>=${myMax}) { 
      /if (${doAnnounce.Length} && ${doVerbose}) {
         /docommand ${doAnnounce} You have maxed your AAs and are unable to purchase any of your defined AAs.
      }
      /if (${AAtoNormal}) {
         /if (${doAnnounce.Length}) {
            /docommand ${doAnnounce} Changing you to full normal XP.
         }
         /alt off
      } 
   } 
/return 


Sub AASelect(string AAName)
   /nomodkey /notify AAWindow ResetFilter leftmouseup
   /nomodkey /notify AAWindow AAW_Subwindows tabselect ${AltAbility[${AAName}].Type}
   /nomodkey /notify AAWindow ${ListNum} listselect ${Window[AAWindow].Child[${ListNum}].List[=${AAName}]}
   /nomodkey /notify AAWindow ${ListNum} leftmouse ${Window[AAWindow].Child[${ListNum}].List[=${AAName}]} 
/return 


Sub AALevelCheck(string AAName)
   /declare lvlStart int local
   /declare lvlEnd int local
   /declare requiredLevel int local
   /declare tChildWindow string local
      
| Live has Description window (which shows Current Level) and DescriptonNext window (which show Next Level), EMU shows Next Level in the Description window
| Let's check if the Next Window exists
   /varset tChildWindow ${If[${Window[AAWindow].Child[AAW_DescriptionNext]},AAW_DescriptionNext,AAW_Description]}
   /call AASelect "${AAName}"
   /varset lvlStart ${Math.Calc[${Window[AAWindow].Child[${tChildWindow}].Text.Find[Level:]} + 7]}
   /if (${lvlStart}) {
      /varset lvlEnd ${Window[AAWindow].Child[${tChildWindow}].Text.Right[${Math.Calc[-${lvlStart}+1]}].Find[,]}
      /if (${lvlEnd}>1) {
         /varset requiredLevel ${Window[AAWindow].Child[${tChildWindow}].Text.Mid[${lvlStart},${Math.Calc[${lvlEnd}-1]}]}
         /if (${requiredLevel}>${Me.Level}) {
            /return ${requiredLevel} 
         }
      }
   }
/return TRUE


| NextIndex doesn't always work, so we will stick with clicking the button
|Sub AAPurchase(string AAName)
|   /if (${AltAbility[${AAName}].NextIndex}>0) {
|      /alt buy ${AltAbility[${AAName}].NextIndex}
|   } else {
|      /alt buy ${AltAbility[${AAName}].ID}
|   }
|   /delay 2
|/return
 
 
Sub AAPurchase 
   /nomodkey /notify AAWindow TrainButton leftmouseup 
| If fast AA purchase is not on, accept the purchase. 
   /if (!${Window[OptionsWindow].Child[OptionsGeneralPage].Child[OGP_AANoConfirmCheckbox].Checked}) { 
      /delay 10s ${Window[ConfirmationDialogBox].Open}
      /if (!${Window[ConfirmationDialogBox].Open}) {
         /echo ${If[${TimeStampOnEcho},[${Time}] ,]}AA confirmation box not showing. Your AA purchase may not have been made.
         /return
      }
      /delay 2 
      /nomodkey /notify ConfirmationDialogBox Yes_Button leftmouseup 
   }
   /delay 2 
/return 


Sub Event_AAEarned
   /varset AACheckTimer
/return 


Sub Event_AACapped
   /if (${AAtoNormal}) {
      /bc Msg: You have reached the AA cap. Changing you to full normal XP.
      /alt off
   } else {
      /bc Msg: You have reached the AA cap.
   } 
/return 


Sub AADump
   /declare a int local 
   /declare b int local
   /declare c int local
   /declare AAcnt int local 1 
   /ini "${AAIniFile}" "${AAIniSection}" AACount 0 
   /declare AALevelStr string local
   /declare AACostStr string local
   /declare AACategory string local
   /declare AATabs string local 35214 
|  /declare AATabs string local 3214 

   /nomodkey /notify AAWindow ResetFilter leftmouseup
   /delay 2

   /for c 1 to 5
      /varset b ${AATabs.Mid[${c},1]}
      /if (!${Window[AAWindow].Child[List${b}]}) /continue
      /varset a 1
      /while (1) {
         /if (!${Window[AAWindow].Child[List${b}].List[${a}].Length}) /break 
         /varset AALevelStr ${Window[AAWindow].Child[List${b}].List[${a}, 2]} 
         /varset AACostStr ${Window[AAWindow].Child[List${b}].List[${a}, 3]} 
         /varset AACategory ${Window[AAWindow].Child[List${b}].List[${a}, 5]} 
         /if (${AACategory.Equal[Expendable]} || (${AALevelStr.Arg[1,"/"]}<${AALevelStr.Arg[2,"/"]} && ${AACostStr.Length} && ${AACostStr.NotEqual[0]})) { 
            /ini "${AAIniFile}" "${AAIniSection}" AA${AAcnt} "${Window[AAWindow].Child[List${b}].List[${a}]}${PipeChar}" 
            /varcalc AAcnt ${AAcnt}+1 
         } 
         /varcalc a ${a}+1 
      } 
   /next c
   /ini "${AAIniFile}" "${AAIniSection}" AACount ${Int[${Math.Calc[${AAcnt} -1]}]} 
   /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Finished dump to file: ${AAIniFile} 
/return


Sub PetToys(int petID)
   /if (!${Defined[DoPetToys]}) /return
   /if (!${DoPetToys} || !${PTCount}) /return
   /if (!${petID}) /return
   /if (${Me.Invis}) {
      /bc Msg: Cannot give toys while Invis
      /return FALSE
   }
   /declare i int local
   /declare j int local
   /declare tPack int local 0
   /declare tItem int local 0
   /declare summonedID int local
   /declare tItemID int local
   /declare tItem1ID int local
   /declare tItem2ID int local

   /if (!${Spawn[pet id ${petID}].ID}) {
      /echo ${If[${TimeStampOnEcho},[${Time}] ,]}PetToys - Pet not found
      /return FALSE
   }
   /if (${Spawn[id ${petID}].Distance3D}>50) {
      /echo ${If[${TimeStampOnEcho},[${Time}] ,]}PetToys - Pet ${Spawn[id ${petID}].Name} too far away.
      /return FALSE
   }
   /if (${Spawn[id ${petID}].Distance3D}>12) {
      /call MBMoveTo ${Spawn[id ${petID}].Y} ${Spawn[id ${petID}].X}
      /if (!${Macro.Return.Equal[TRUE]}) {
         /echo ${If[${TimeStampOnEcho},[${Time}] ,]}PetToys - Problem getting to ${Spawn[id ${petID}].Name}
         /return FALSE
      }
   }
   /call ClearCursor force
   /for i 1 to ${Me.NumBagSlots}
      /if (!${Me.Inventory[pack${i}].ID}) {
         /varset tPack ${i}
         /break
      }
      /if (${Me.Inventory[pack${i}].Items}) /continue
      /if (${Me.FreeInventory}>1) /varset tPack ${i}
   /next i
   /if (!${tPack}) {
      /echo ${If[${TimeStampOnEcho},[${Time}] ,]}PetToys - No inventory slot for pet toys.
      /return FALSE
   }
   /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Using pack ${tPack} for toys
   /for i 1 to ${PTCount}
      /if (!${DoPetToys}) {
         /echo ${If[${TimeStampOnEcho},[${Time}] ,]}PetToysOff - aborting
         /return FALSE
      }
      
      /if (${Me.Pet.ID}==${petID}) {
         /if (${Spell[${PTSpell[${i}].Arg[1,${PipeChar}]}].Level}>=76 && (${PTSpell[${i}].Find[muzzle]} || ${PTSpell[${i}].Find[visor]} || ${PTSpell[${i}].Find[belt]})) /continue
      } else {
         /bc Msg: Giving toys to ${Spawn[id ${petID}].CleanName} (${i} of ${PTCount})
      }
      /echo PetToys ${i}
      /if (${PTSpell[${i}].Find[${PipeChar}item]}) {
            /echo ${FindItem[${PTSpell[${i}].Arg[1,${PipeChar}]}].ID}
            /echo /casting ${FindItem[${PTSpell[${i}]}].ID}
            /casting ${FindItem[${PTSpell[${i}].Arg[1,${PipeChar}]}].ID}
            /echo Casting item.
            /delay 15s ${Cursor.ID}
            /varset summonedID ${Cursor.ID}
            /call Give ${Cursor.ID} ${petID} 1
            /delay 10s !${FindItemCount[${summonedID}]}
            /delay 10
            /goto :nextItem
      } else {
      /if (${Me.Book[${PTSpell[${i}].Arg[1,${PipeChar}]}]}) {
         {
            /call MQ2Cast "${PTSpell[${i}].Arg[1,${PipeChar}]}" ${PTSpell[${i}].Arg[2,${PipeChar}]} 10s -targetID${PipeChar}${Me.ID} -maxtries${PipeChar}3
         }
         /delay 15s ${Cursor.ID}
         /while (1) {
            /delay 1
            /if (!${Cursor.ID}) {
               /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Problem with summoning pettoys - aborting
               /return FALSE
            }
            /if (${Cursor.NoRent}) /break
            /if (${Cursor.ID}) /autoinv
            /delay 5s ${Cursor.ID}
         }
         /varset summonedID ${Cursor.ID}
         /delay 5
         /nomodkey /itemnotify pack${tPack} leftmouseup
         /delay 5s ${Cursor.ID}!=${summonedID}
         /delay 5
         /if (${Cursor.ID}) /call ClearCursor force
         /delay 5
         /if ((${Me.Inventory[pack${tPack}].ID}!=${summonedID}) || ${Cursor.ID}) {
            /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Problem with pettoys - aborting
            /return FALSE
         }
         /if (${Me.Inventory[pack${tPack}].Name.Find[folded]}) {
            /nomodkey /itemnotify pack${tPack} rightmouseup
            /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Opening ${Me.Inventory[pack${tPack}].Name}
            /delay 15s ${Cursor.ID}
            /if (!${Cursor.ID}) /return FALSE
            /delay 1s
            /nomodkey /itemnotify pack${tPack} leftmouseup
            /delay 5
            /delay 5s ${Me.Inventory[pack${tPack}].ID}
            /if (!${Me.Inventory[pack${tPack}].Name.Find[Phantom Satchel]} && !${Me.Inventory[pack${tPack}].Name.Find[Pouch of Quellious]}) {
               /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Problem with pettoys pack - aborting
               /return FALSE
            }
            /if (${Cursor.ID}) /call ClearCursor force
            /delay 5
         }
         /if (${Me.Inventory[pack${tPack}].Container}) {
            /if (${Me.Inventory[pack${tPack}].Name.Find[Phantom Satchel]} || ${Me.Inventory[pack${tPack}].Name.Find[Pouch of Quellious]}) {
               /nomodkey /itemnotify pack${tPack} rightmouseup
               /delay 1s
               /for tItem 1 to ${Me.Inventory[pack${tPack}].Container}
                  /if (!${DoPetToys}) {
                     /echo ${If[${TimeStampOnEcho},[${Time}] ,]}PetToys Off - aborting
                     /return FALSE
                  }
                  /if (${Me.Inventory[pack${tPack}].Item[${tItem}].ID}) {
                     /if (${PTItem1[${i}].Length}) {
                        /if (!${Me.Inventory[pack${tPack}].Item[${tItem}].Name.Equal[${PTItem1[${i}]}]} && !${Me.Inventory[pack${tPack}].Item[${tItem}].Name.Equal[${PTItem2[${i}]}]}) /continue
                     }
                     /varset tItemID ${Me.Inventory[pack${tPack}].Item[${tItem}].ID}
                     /call PickUpItem ${tItemID} ${Me.Inventory[pack${tPack}].Item[${tItem}].ItemSlot} ${Me.Inventory[pack${tPack}].Item[${tItem}].ItemSlot2} 1                       
                     /if (${Macro.Return.Equal[false]}) {
                        /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Problem with PetToys - PickUpItem failed
                        /return FALSE
                     }
                     /call Give ${tItemID} ${petID} 1
                     /if (${Macro.Return.Equal[false]}) {
                        /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Problem with PetToys - Give failed
                        /return FALSE
                     }
                     /if (${Cursor.ID}) /call PetToyDestroy
                  }
               /next tItem
               /delay 5
               /if (${Me.Inventory[pack${tPack}].Items}) {
                  /for tItem 1 to ${Me.Inventory[pack${tPack}].Container}
                     /if (!${Me.Inventory[pack${tPack}].Item[${tItem}].NoRent} && ${Me.Inventory[pack${tPack}].Item[${tItem}].Name.Length}) {
                        /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Bag has non summoned item(s) in it. Aborting delete. Setting DoPetToys FALSE
                        /varset DoPetToys FALSE
                        /return FALSE
                     }
                  /next tItem
               }
               /nomodkey /itemnotify pack${tPack} leftmouseup
               /delay 5s ${Cursor.ID}
               /if (${Cursor.Name.Find[Phantom Satchel]} || ${Cursor.Name.Find[Pouch of Quellious]}) /destroy
               /delay 1s !${Cursor.ID}
            } else {
               /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Problem with pettoys - Unexpected Container
               /return FALSE
            }
         } else /if (${Me.Inventory[pack${tPack}].Name.Find[Summoned:]}) {
            /varset tItemID ${Me.Inventory[pack${tPack}].ID}
            /call PickUpItem ${tItemID} ${Me.Inventory[pack${tPack}].ItemSlot} ${Me.Inventory[pack${tPack}].ItemSlot2} 1                       
            /if (${Macro.Return.Equal[false]}) {
               /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Problem with PetToys - PickUpItem failed
               /return FALSE
            }
            /call Give ${tItemID} ${petID} 1
            /if (${Macro.Return.Equal[false]}) {
               /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Problem with PetToys - Give failed
               /return FALSE
            }
            /if (${Cursor.ID}) /call PetToyDestroy
         } else {
            /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Problem with pettoys - Unexpected Item
            /return FALSE
         }
      }
      }
:nextItem
   /next i
   /if (${Me.Pet.ID}!=${petID}) /bc Msg: Finished giving toys to ${Spawn[id ${petID}].CleanName}
   /if (!${InvSlot[pack8].Item.ID}) /call ResetBag
/return TRUE


| Check for Stuck Gems
Sub StuckGemsCheck
   /if (!${DoStuckGems}) /return
   /if (!${Me.Casting.ID} || !${GemStuckID} || ${Me.Casting.ID}!=${GemStuckID}) {
      /varset GemStuckTimer 25s
      /varset GemStuckID ${Me.Casting.ID}
   } else /if (!${GemStuckTimer}) {
      /if (${MBDebug}) {
         /echo ${If[${TimeStampOnEcho},[${Time}] ,]} GSb - ${Time} - ${Me.Casting.ID} - ${GemStuckID}
      }
      /call StuckGems
      /if (${MBDebug}) {
         /echo ${If[${TimeStampOnEcho},[${Time}] ,]} GSc - ${Time} - ${Me.Casting.ID} - ${GemStuckID}
      }
   }
/return


Sub PetToyDestroy
   /if (${Cursor.ID} && (${Cursor.NoRent} || ${Cursor.Name.Find[muzzle]} || ${Cursor.Name.Find[visor]} || ${Cursor.Name.Find[belt]})) {
      /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Pet didn't need ${Cursor.Name} - Destroying
      /destroy
      /delay 10
   }
/return


| Stuck Gems
Sub StuckGems
   /if (!${Me.Casting.ID}) /return
   /if (!${DoStuckGems}) {
      /if (!${GemStuckRetry}) {
         /echo ${If[${TimeStampOnEcho},[${Time}] ,]}My Spell Gems may be Stuck, but DoStuckGems is FALSE.
         /varset GemStuckRetry 3s
      }
      /return
   }
   /if (!${GemStuckRetry}) {
      /docommand /${BeepCommand}
      /bc ${If[${TimeStampOnEcho},[${Time}] ,]}My Spell Gems may be Stuck
      /echo ${If[${TimeStampOnEcho},[${Time}] ,]}My Spell Gems may be Stuck
      /delay 1s
      /if (${StuckGemsAA.Length}>2) {
         /if (${Me.AltAbilityReady[${StuckGemsAA}]}) {
            /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Attempting to unstick with ${StuckGemsAA}.
            /tar id ${Me.ID}
            /alt act ${Me.AltAbility[${StuckGemsAA}].ID}
            /delay 5s !${Me.Casting.ID}
         } else {
            /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Waiting for ${StuckGemsAA} to be ready so we can unstick the gems.
         }
      } else {
         /echo ${If[${TimeStampOnEcho},[${Time}] ,]}You have not set StuckGemsAA in your ini, so we can't unstick you.
      }
      /varset GemStuckRetry 2s
   }
/return


Sub EditTwist(string tTwistList,string tGem,string tAction)
   /declare a int local
   /declare b int local
   /declare tList string local
   /declare sReturn string local
   /if (!${Select[${tGem},${ValidSpellGems}]} || !${Select[${tAction},add,delete]}) {
      /varset sReturn ${tTwistList}
   } else /if (${tAction.Equal[add]}) {
      /if (!${tTwistList.Find[${tGem}]}) {
         /varset sReturn ${tTwistList} ${tGem}
      } else /if (!${Select[${tGem},1,2,3]}) {
         /varset sReturn ${tTwistList}
      } else {
         /varset b ${Math.Calc[${tTwistList.Count[ ]}+1]}
         /for a 1 to ${b}
            /if (${tTwistList.Arg[${a}].Equal[${tGem}]}) /break
         /next a
         /if (${a}>${b}) {
            /varset sReturn ${tTwistList} ${tGem}
         } else {
            /varset sReturn ${tTwistList}
         }
      }
   } else {
      /if (!${tTwistList.Find[${tGem}]}) {
         /varset sReturn ${tTwistList}
      } else {
         /varset tList
         /for a 1 to ${tTwistList.Count[ ]}+1
            /if (!${tTwistList.Arg[${a}].Equal[${tGem}]}) /varset tList ${tList} ${tTwistList.Arg[${a}]} 
         /next a
         /varset sReturn ${tList}
      }
   }
/return ${sReturn}


|Get Bard Slot for item (and AA's with CastTime) casting with Twist
Sub GetBardSlot(string tName,string tCast)
   /declare tCastClass string local
   /declare tCastGem string local
   /declare a int local
   /if (${Me.Class.ShortName.NotEqual[BRD]}) /return
   /varset tCastClass ${tCast.Arg[1,${PipeChar}]}
   /if (!${Select[${tCastClass},AD,AH,AB,AQ,AE]}) /return
   /varset tCastGem ${tCast.Arg[2,${PipeChar}]}
   /if (${${tCastClass}Gem[${tCastGem}].Equal[item]}) {
      /if (${FindItem[=${tName}].ID}) {
         /if (${NextBardSlot}>20) {
            /varcalc a ${FindItem[=${tName}].CastTime}/100+5
            /squelch /twist set ${NextBardSlot} ${a} 5 "${tName}" DISABLED
            /varset ${tCastClass}BardSlot[${tCastGem}] ${NextBardSlot}
            /varcalc NextBardSlot ${NextBardSlot}-1
         }
      }
   } else /if (${${tCastClass}Gem[${tCastGem}].Equal[alt]}) {
      /if (${Me.AltAbility[${tName}].Spell.MyCastTime}) {
         /if (${NextBardSlot}>20) {
            /varcalc a ${Me.AltAbility[${tName}].Spell.MyCastTime}/100+5
            /squelch /twist set ${NextBardSlot} ${a} 5 "${tName}" AA
            /varset ${tCastClass}BardSlot[${tCastGem}] ${NextBardSlot}
            /varcalc NextBardSlot ${NextBardSlot}-1
         }
      }
   }
/return

 
Sub MemGem(int tGem,string sSpell)
   /declare tReturn string local
   /if (${Me.Gem[${tGem}].Name.Equal[${sSpell}]}) {
      /return TRUE
   }   
   /if (${Cursor.ID}) {
      /call ClearCursor force
   }
   /if (${Me.Class.ShortName.NotEqual[BRD]}) {
      /delay 2s ${Cast.Status.Equal[i]}
   }   
   /memspell ${tGem} "${sSpell}"
   /delay 2s ${Window[SpellBookWnd].Open}
   /delay 12s ${Me.Gem[${tGem}].Name.Equal[${sSpell}]} || !${Window[SpellBookWnd].Open}
   /delay 1
   /if (${Me.Gem[${tGem}].Name.Equal[${sSpell}]}) {
      /varset tReturn TRUE
   } else /if (!${Window[SpellBookWnd].Open}) {
      /varset tReturn INTERRUPTED
   } else {
      /bc Msg: FAILED MEMORIZE - Gem ${tGem} - ${sSpell}
      /varset tReturn FALSE
   }
   /if (${Window[SpellBookWnd].Open}) /book close
/return ${tReturn}


Sub SetPuller
   /if (${ManualPuller} && ${AllPullerName.Length}<3) /return
   /declare PrevPullerName string local ${PullerName}
   /declare s string local
   /declare i int local
   /if (!${ManualPuller} && ${Group.Puller.Name.Length} && !${Group.Puller.Type.Equal[corpse]} && !${Group.Puller.Offline}) { 
      /varset s ${Group.Puller.Name}
   } else /if (${AllPullerName.Length}>2) {
      /if (!${ManualPuller} || !${PullerName.Length} || ${Spawn[${PullerName}].Type.Equal[corpse]}) { 
         /for i 1 to ${Math.Calc[${AllPullerName.Count[,]}+1]}
            /if (${Spawn[pc =${AllPullerName.Arg[${i},,]}].ID} && !${Spawn[pc =${AllPullerName.Arg[${i},,]}].Type.Equal[corpse]}) {
               /varset s ${AllPullerName.Arg[${i},,]}
               /break
            }
         /next i
      }
   }   
   /if (${s.Length}) {
      /varset PullerName ${s.Left[1].Upper}${s.Right[-1]}
      /if (!${PullerName.Equal[${PrevPullerName}]}) {
         /bc Msg: ${PullerName} is now PULLER
      }
   }
/return


Sub SetTank
   /if (${ManualTank} && ${AllTankName.Length}<3) {
      /call SetTankID
      /return
   }
   /declare PrevTankName string local ${TankName}
   /declare s string local
   /declare i int local
   /if (!${ManualTank} && ${Group.MainAssist.Name.Length} && !${Group.MainAssist.Type.Equal[corpse]} && !${Group.MainAssist.Offline}) { 
      /varset s ${Group.MainAssist.Name}
   } else /if (!${ManualTank} && ${Group.MainTank.Name.Length} && !${Group.MainTank.Type.Equal[corpse]} && !${Group.MainTank.Offline}) { 
      /varset s ${Group.MainTank.Name}
   } else /if (${AllTankName.Length}>2) {
      /if (!${ManualTank} || !${TankName.Length} || ${Spawn[${TankName.Arg[1,${PipeChar}]}].Type.Equal[corpse]}) { 
         /for i 1 to ${Math.Calc[${AllTankName.Count[,]}+1]}
            /if (${Spawn[pc =${AllTankName.Arg[${i},,].Arg[1,${PipeChar}]}].ID} && !${Spawn[pc =${AllTankName.Arg[${i},,].Arg[1,${PipeChar}]}].Type.Equal[corpse]}) {
               /varset s ${AllTankName.Arg[${i},,]}
               /break
            }
         /next i
      }
   }
   /if (${s.Length}) {
      /varset TankName ${s.Left[1].Upper}${s.Right[-1]}
      /if (!${TankName.Equal[${PrevTankName}]}) {
         /bc Msg: ${TankName.Arg[1,${PipeChar}]}${If[!${TankName.Arg[2,${PipeChar}].Equal[pet]},,'s Pet]} is now TANK
      }
   }
   /call SetTankID
/return


Sub SetTankID
   /if (${TankName.Length}<3) {
      /varset TankID 0
   } else {
      /varset TankID ${If[${Spawn[pc =${TankName.Arg[1,${PipeChar}]}].ID},${Spawn[pc =${TankName.Arg[1,${PipeChar}]}]${If[!${TankName.Arg[2,${PipeChar}].Equal[pet]},,.Pet]}.ID},${Spawn[mercenary =${TankName.Arg[1,${PipeChar}]}].ID}]}
   }
|/echo -${TankName}-   -${TankID}-
/return


| MQ2Cast Spell Routines - Highly customized for ModBot
|
| These are much like the code from Spell_Routines.inc by Rusty. This uses the MQ2Cast Plugin by s0rcier.
Sub MQ2Cast(string spellName,string spellType,timer giveUpTimer,string mySub,string MQ2Feature1,string MQ2Feature2,string MQ2Feature3,string MQ2Feature4)
|/echo -${spellName}- -${spellType}- -${giveUpTimer}- -${mySub}-
   /declare i int local
   /declare bandSet string local
   /declare oldItem string local

   /varset interruptFlag FALSE

   /if (${mySub.Left[1].Equal[-]}) {
      /varset MQ2Feature4 ${MQ2Feature3}
      /varset MQ2Feature3 ${MQ2Feature2}
      /varset MQ2Feature2 ${MQ2Feature1}
      /varset MQ2Feature1 ${mySub}
      /varset mySub
   }
   /if (${Corpse.Open}) {
      /nomodkey /keypress esc
   }
   /if (${Me.Feigning}) {
      /if (!${noFeigning}) /stand
   }
   /if (${Window[SpellBookWnd].Open}) /book close
   /if (${spellType.Equal[item]}) {
      /if (${FindItem[=${spellName}].EffectType.Equal[Click Worn]}) {
         /call SREquipItem "${spellName}"
         /varset oldItem ${Macro.Return}
      }
   }
   /if (${Stick.Status.Equal[on]}) {
      /squelch /stick pause
      /delay 5 !${Me.Moving}
   }
   /if (${AdvPath.Following} && !${AdvPath.Paused}) {
|/echo pausing in cast
      /varset FollowState 3
      /squelch /afollow pause
   }
   /for i 1 to 4
      /if (!${MQ2Feature${i}.Length}) /break
      /if (${MQ2Feature${i}.Find[-bandolier${PipeChar}]}) {
         /varset bandSet ${MQ2Feature${i}.Arg[2,${PipeChar}]}
         /if (${bandSet.Length}) /call SRBandIn ${bandSet}
         /break
      }
   /next i
| Since there are other things that may be casting/activating (like MQ2Melee things), it is possible to be here and find MQ2Cast is no longer ready. Wait for it.
   /if (${Me.Class.ShortName.NotEqual[BRD]} && ${Cast.Status.NotEqual[i]}) {
      /while (${Cast.Status.NotEqual[i]}) {
|        /if (${MBDebug}) {
|           /echo ${If[${TimeStampOnEcho},[${Time}] ,]}SRa - ${Time} - ${Macro.RunTime} - ${MBCurrentSpell} - ${interruptFlag} - ${Cast.Status} - ${Cast.Result} - ${Cast.Stored} - ${NetBots[${TankName.Arg[1,|]}].TargetID} - ${Me.Casting.ID}
|        }
         /if (!${giveUpTimer}) {
            /call StuckGems
            /if (${Me.Casting.ID}) /return CAST_NOTREADY
         }
         /delay 1
      }
      /varset giveUpTimer ${giveUpTimer.OriginalValue}
   }
   /if (${noInvis}) {
      /squelch /casting "${spellName}${PipeChar}${spellType}"  "-invis" "${MQ2Feature1}" "${MQ2Feature2}" "${MQ2Feature3}" "${MQ2Feature4}"
   } else {
      /squelch /casting "${spellName}${PipeChar}${spellType}"  "${MQ2Feature1}" "${MQ2Feature2}" "${MQ2Feature3}" "${MQ2Feature4}"
   }

   /varset SpellWasCast TRUE
   /while (${Cast.Status.Find[C]}) {
      /if (!${giveUpTimer}) {
         /if (${MBDebug}) {
            /echo ${If[${TimeStampOnEcho},[${Time}] ,]}no SRtimer - ${Time} - ${Macro.RunTime} - ${MBCurrentSpell} - ${interruptFlag} - ${Cast.Status} - ${Cast.Result} - ${Cast.Stored} - ${NetBots[${TankName.Arg[1,${PipeChar}]}].TargetID} - ${Me.Casting.ID}
         }
         /call StuckGems
         /delay 1
      }
      /if (${mySub.Length}) /call ${mySub} ${giveUpTimer} 
      /if (${interruptFlag} || !${MBCurrentSpell.Length}) /break
   }

   /if (${MBDebug}) {
      /if (${Cast.Status.NotEqual[i]} && !${interruptFlag}) /echo ${If[${TimeStampOnEcho},[${Time}] ,]}SR1 - ${Time} - ${MBCurrentSpell} - ${interruptFlag} - ${Cast.Status} - ${Cast.Result} - ${Cast.Stored} - ${NetBots[${TankName.Arg[1,${PipeChar}]}].TargetID}
   }

   /if (${Stick.Status.Equal[paused]}) /squelch /stick unpause

   /if (${FollowState}==3) {
|/echo unpause after cast
      /varset FollowState 1
      /squelch /afollow unpause
   }

   /if (${bandSet.Length}) {
      /call SRBandOut
   }
   /if (${oldItem.Length}) {
      /call SREquipItem ${oldItem}
   }

   /varset castReturn ${Cast.Result}

/return ${Cast.Result}


Sub Interrupt
   /stopcast
   /varset interruptFlag TRUE
/return


Sub SRBandIn(setName)
   /if (!${Defined[bandIni]}) /declare bandIni string local ..\MQ2Bandolier_${Me.Name}.ini
   /declare equipSlots string local
   /declare itemSlot string local
   /declare itemNum string local
   /declare i int local
        
   /varset equipSlots ${Ini["${bandIni}",${setName}]}

   /if (${equipSlots.Length} && ${equipSlots.Count[${PipeChar}]} > 1) {
      /for i 1 to ${numSlots}
         /if (${Me.Inventory[${Math.Calc[${i}-1]}].ID}) {
            /varset equippedArray[${i}] ${Me.Inventory[${Math.Calc[${i}-1]}].Name}
         } else {
            /varset equippedArray[${i}]
         }
      /next i 
      /for i 1 to ${equipSlots.Count[${PipeChar}]} - 1
         /varset itemSlot ${equipSlots.Arg[${i},${PipeChar}]}
         /if (${itemSlot.Length}) {
            /varset itemNum ${Ini["${bandIni}",${setName},${itemSlot}]}
            /if (${itemNum.Length}) {
               /if (${Me.Inventory[${itemSlot}].ID} != ${itemNum}) /call SRSwapItemNum ${itemNum} ${itemSlot}
            }
         }
      /next i
   }
/return


Sub SRBandOut
   /declare i int local
   /for i 1 to ${numSlots}
      /if (${equippedArray[${i}].Length}) {
         /if (!${equippedArray[${i}].Equal[${Me.Inventory[${Math.Calc[${i}-1]}].Name}]}) {
            /call SRSwapItem "${equippedArray[${i}]}" ${slotNames.Arg[${i},,]}
         }
      }
   /next i 
/return


Sub SRClearCursor
   /declare i int local
:check_cursor
   /if (${Cursor.ID}) {
      /if (!${Cursor.Container} || !${Cursor.Items}) /timed 5 /autoinventory
      /if (${Cursor.Container}) {
         /for i 1 to ${Me.NumBagSlots}    
            /if (!${Me.Inventory[pack${i}].Container}) /nomodkey /itemnotify pack${i} leftmouseup
         /next i
      } 
      /goto :check_cursor
   }
/return


Sub SREquipItem(WhatWhere)
   /declare destName string local
   /declare itemName string local ${WhatWhere.Arg[1,${PipeChar}]}
   /declare slotName string local ${WhatWhere.Arg[2,${PipeChar}]}
    
   /if (${slotName.Equal[NULL]}) /varset slotName ${FindItem[=${itemName}].WornSlot[1].Name}

   /if (${FindItem[=${itemName}].itemSlot}<${numSlots} || !${FindItem[=${itemName}].WornSlot[${slotName}]}) /return

   /if (${Me.Inventory[${slotName}].ID}) /varset destName "${Me.Inventory[${slotName}].Name}${PipeChar}${slotName}"

   /call SRSwapItem "${itemName}" "${slotName}"
/return ${destName}


Sub SRSwapItem(itemName,slotName)
   /if (${Cursor.ID}) /call SRClearCursor
   /exchange "${itemName}" ${slotName}
   /delay 5s ${Me.Inventory[${slotName}].Name.Equal[${itemName}]}
   /delay 5s !${Cursor.ID}
   /if (${Cursor.ID}) /call SRClearCursor
/return


Sub SRSwapItemNum(itemNum,slotNum)
   /if (${Cursor.ID}) /call SRClearCursor
   /exchange ${itemNum} ${slotNum}
   /delay 5s ${Me.Inventory[${slotNum}].ID}==${itemNum}
   /delay 5s !${Cursor.ID}
   /if (${Cursor.ID}) /call SRClearCursor
/return

| End of - MQ2Cast Spell Routines


Sub TFToggle(string sVar,string sSetting)
   /varset ${sVar} ${If[${Select[${sSetting},on,true]},TRUE,${If[${Select[${sSetting},off,false]},FALSE,${If[${${sVar}},FALSE,TRUE]}]}]}
   /bc Setting - ${sVar} ${${sVar}}
/return


Sub Command_ABORT
   /varset ACMATarget 0
   /if (${ACState}) {
      /varcalc ACState ${ACState}+2
      /call AdvCombat
   }
   /if (${Me.Casting.ID}) {
      /if (${Me.Class.ShortName.Equal[BRD]}) /squelch /stoptwist
      /stopcast
      /if (${Me.Class.ShortName.Equal[BRD]}) /squelch /stoptwist
   }
   /if (${Me.Mount.ID}) /dismount
   /varset DoMelee FALSE
   /varset DoDebuffs FALSE
   /varset DoHeals FALSE
   /if (!${Me.Class.ShortName.Equal[brd]}) /varset DoBuffs FALSE
   /bc Setting - DoHeals, DoBuffs, DoDebuffs & DoMelee FALSE and Following ${SenderName}
   /call Command_FOLLOW "follow close"
/return


Sub Command_ADDBUFF
   /declare MsgText string local ${CommandText}
   /if (${MsgText.Arg[2].Arg[1,${PipeChar}].Length} && ${MsgText.Arg[2].Arg[2,${PipeChar}].Length}) {
      /varcalc ABCount ${ABCount}+1
      /ini "${IniFile}" AdvBuff ABCount ${ABCount}
      /echo ${If[${TimeStampOnEcho},[${Time}] ,]}${MsgText.Right[-8]} : ABCount = ${ABCount}
      /if (${ABNewFormat}) {
         /ini "${IniFile}" AB${ABCount} Gem ${MsgText.Arg[2].Arg[2,${PipeChar}]}
         /ini "${IniFile}" AB${ABCount} Spell "${MsgText.Arg[2].Arg[1,${PipeChar}]}"
         /if (${MsgText.Arg[3].Length}) /ini "${IniFile}" AB${ABCount} SpellAlias "${MsgText.Arg[3]}"
         /if (${MsgText.Arg[4].Length}) /ini "${IniFile}" AB${ABCount} TarType "${MsgText.Arg[4]}"
         /if (${MsgText.Arg[5].Length}) /ini "${IniFile}" AB${ABCount} TarCnt "${MsgText.Arg[5]}"
      } else {
         /ini "${IniFile}" AdvBuff ABGem${ABCount} ${MsgText.Arg[2].Arg[2,${PipeChar}]}
         /ini "${IniFile}" AdvBuff ABSpell${ABCount} "${MsgText.Arg[2].Arg[1,${PipeChar}]}"
         /if (${MsgText.Arg[3].Length}) /ini "${IniFile}" AdvBuff ABSpellAlias${ABCount} "${MsgText.Arg[3]}"
         /if (${MsgText.Arg[4].Length}) /ini "${IniFile}" AdvBuff ABTarType${ABCount} "${MsgText.Arg[4]}"
         /if (${MsgText.Arg[5].Length}) /ini "${IniFile}" AdvBuff ABTarCnt${ABCount} "${MsgText.Arg[5]}"
      }
      /echo Restart to add to memory.
   } else {
      /echo Invalid AddBuff String. Buff Not Added.
   }
/return


Sub Command_ADDDEBUFF
   /declare MsgText string local ${CommandText}
   /if (${MsgText.Arg[2].Arg[1,${PipeChar}].Length} && ${MsgText.Arg[2].Arg[2,${PipeChar}].Length}) {
      /varcalc ADCount ${ADCount}+1
      /ini "${IniFile}" AdvDebuff ADCount ${ADCount}
      /echo ${If[${TimeStampOnEcho},[${Time}] ,]}${MsgText.Right[-10]} : ADCount = ${ADCount}
      /if (${ADNewFormat}) {
         /ini "${IniFile}" AD${ADCount} Gem ${MsgText.Arg[2].Arg[2,${PipeChar}]}
         /ini "${IniFile}" AD${ADCount} Spell "${MsgText.Arg[2].Arg[1,${PipeChar}]}"
         /if (${MsgText.Arg[3].Length}) /ini "${IniFile}" AD${ADCount} SpellAlias "${MsgText.Arg[3]}"
         /if (${MsgText.Arg[4].Length}) /ini "${IniFile}" AD${ADCount} TarType "${MsgText.Arg[4]}"
         /if (${MsgText.Arg[5].Length}) /ini "${IniFile}" AD${ADCount} TarCnt "${MsgText.Arg[5]}"
      } else {
         /ini "${IniFile}" AdvDebuff ADGem${ADCount} ${MsgText.Arg[2].Arg[2,${PipeChar}]}
         /ini "${IniFile}" Advdebuff ADSpell${ADCount} "${MsgText.Arg[2].Arg[1,${PipeChar}]}"
         /if (${MsgText.Arg[3].Length}) /ini "${IniFile}" Advdebuff ADSpellAlias${ADCount} "${MsgText.Arg[3]}"
         /if (${MsgText.Arg[4].Length}) /ini "${IniFile}" Advdebuff ADTarType${ADCount} "${MsgText.Arg[4]}"
         /if (${MsgText.Arg[5].Length}) /ini "${IniFile}" Advdebuff ADTarCnt${ADCount} "${MsgText.Arg[5]}"
      }
      /echo Restart to add to memory.
   } else {
      /echo Invalid AddDebuff String. Debuff Not Added.
   }
/return


Sub Command_ADDTANK
   /declare MsgText string local ${CommandText}
   /declare tName string local
   /declare tList string local
   /if (!${MsgText.Arg[2].Length}) {
      /varset tName ${Me.Name}
   } else {
      /varset tName ${MsgText.Arg[2]}
   }
   /echo Adding ${tName} to tank list
   /if (${tName.Length}>2 && !${TankList.Find[${tName}]}) {
      /varset tList ${TankList} ${tName}
      /docommand /mb setinivar TankList "${tList}"
   }
/return


Sub Command_ASSIST
   /declare MsgText string local ${CommandText}
   /if (${MsgText.Arg[2].Length}>1) {
      /varset ACAssistPct ${MsgText.Arg[2]}
   } else {
      /if (!${TankName.Arg[1,${PipeChar}].Equal[${Me.CleanName}]} && ${Spawn[=${SenderName}].ID}) {
         /target ${SenderName}
         /delay 1s ${Target.ID}==${Spawn[${SenderName}].ID}
         /assist
         /delay 1s ${Target.ID}!=${Spawn[${SenderName}].ID}
         /delay 5 ${Target.ID}==${NetBots[${SenderName}].TargetID}
         /if (${AdvPath.Following}) /afollow off
         /killthis
         /delay 1s ${Stick.Status.Equal[on]}
         /if (${Stick.Status.Equal[off]} && ${Melee.Combat}) /stick ${DefaultStick}
      }
   }
/return


Sub Command_ATTACK
   /declare MsgText string local ${CommandText}
   /bc Setting - DoMelee TRUE & Attacking
   /varset ACState 100
   /varset DoMelee TRUE
   /if (!${MsgText.Arg[2].Length}) /varset MsgText ${MsgText} ${Me.CleanName}
   /call AdvCombat ${MsgText.Right[-7]}
/return


Sub Command_AUTOBANK
   /declare MsgText string local ${CommandText}
   /if (!${MsgText.Arg[2].Length}) {
      /call Help AutoBank
      /return FALSE
   }
   /if (!${Window[BigBankWnd].Open}) {
      /call OpenBank
      /if (!${Window[BigBankWnd].Open}) /return FALSE
   }
   /declare ItemNames string local ${MsgText.Arg[2]}
   /echo ${If[${TimeStampOnEcho},[${Time}] ,]}AutoBanking
   /declare a int local 0
   /declare iCount int local 0
   /declare tPack int local
   /declare tItem int local
   /declare tItemID int local
   /declare tItemName string local
   /for a 1 to ${Math.Calc[${ItemNames.Count[${PipeChar}]}+1]}
      /if (${Category.Find[${ItemNames.Arg[${a},${PipeChar}]}]}) {
| Search for items marked as Category in loot.ini file.
         /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Searching inventory for items marked ${ItemNames.Arg[${a},${PipeChar}]} in your loot file.
         /for tPack 1 to ${Me.NumBagSlots}
            /if (!${Me.Inventory[pack${tPack}].ID}) /continue
            /for tItem ${If[${Me.Inventory[pack${tPack}].Container},1,0]} to ${Me.Inventory[pack${tPack}].Container}
               /if (!${Me.Inventory[pack${tPack}]${If[${tItem},.Item[${tItem}],]}.ID}) /continue
               /varset tItemName ${Me.Inventory[pack${tPack}]${If[${tItem},.Item[${tItem}],]}.Name}
               /if (${Ini["${LootIni}","${tItemName.Left[1]}","${tItemName}"].Find[${ItemNames.Arg[${a},${PipeChar}]}]}) {
                  /varset tItemID ${Me.Inventory[pack${tPack}]${If[${tItem},.Item[${tItem}],]}.ID}
                  /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Autobanking matching item: ${tItemName} (${FindItemCount[${tItemID}]})
                  /call BankThemAll ${tItemID}
                  /if (${Macro.Return.Equal[false]}) /return FALSE
               }
            /next tItem
         /next tPack       
      } else {
         /varset tItemID ${FindItem[=${ItemNames.Arg[${a},${PipeChar}]}].ID}
         /varset tItemName ${FindItem[=${ItemNames.Arg[${a},${PipeChar}]}].Name}
         /if (${tItemID}) {
            /echo ${If[${TimeStampOnEcho},[${Time}] ,]}AutoBanking - ${tItemName} (${FindItemCount[${tItemID}]})
            /call BankThemAll ${tItemID}
            /if (${Macro.Return.Equal[false]}) /return FALSE
         } else {
            /echo ${If[${TimeStampOnEcho},[${Time}] ,]}No ${ItemNames.Arg[${a},${PipeChar}]}'s found
         }
      }
   /next a
   /delay 1
   /nomodkey /notify BigBankWnd BIGB_DoneButton leftmouseup
   /delay 5
   /echo ${If[${TimeStampOnEcho},[${Time}] ,]}AutoBanking Complete   
/return TRUE


Sub Command_AUTOSKILLS
| List the combat skills you currently have on /autoskill.
   /declare used int local
   /declare x int local
| Currently, only two concurrent skills allowed
   /for x 1 to 2
      /if (${Me.AutoSkill[${x}].Name.NotEqual[NULL]}) {
         /varcalc used ${used}+1
         /echo ${If[${TimeStampOnEcho},[${Time}] ,]}AutoSkill ${used} -> ${Me.AutoSkill[${x}]}
      }
   /next x
   /if (${used}<1) /echo ${If[${TimeStampOnEcho},[${Time}] ,]}No AutoSkills in use
/return


Sub Command_BACKOFF
   /declare MsgText string local ${CommandText}
   /if (${ACState}) {
      /varcalc ACState ${ACState}+2
      /call AdvCombat
   }
   /squelch /melee reset
   /varset ACMATarget 0
   /call BackPets
   /if (${MsgText.Equal[backoff pause]}) {
      /bc Setting - MBPause TRUE
      /varset MBPause TRUE
   }
   /bc Msg: Backing off
/return


Sub Command_BUFFUP
   /bc Setting - DoBuffs & DoEvents & DoHeals TRUE
   /varset DoEvents TRUE
   /varset DoHeals TRUE
   /varset DoBuffs TRUE
/return


Sub Command_BUY
   /declare MsgText string local ${CommandText}
   /if (!${MsgText.Arg[2].Length}) {
      /call Help Buy
      /return
   }
   /if (${MsgText.Arg[3].Equal[max]}) {
      /call Buy ""${MsgText.Arg[2]}${PipeChar}max"" ${If[${MsgText.Arg[4].Length},""${MsgText.Arg[4]}"",]} ${If[${MsgText.Arg[5].Length},""${MsgText.Arg[5]}"",]}
   } else {
      /call Buy ""${MsgText.Arg[2]}"" ${If[${MsgText.Arg[3].Length},""${MsgText.Arg[3]}"",]} ${If[${MsgText.Arg[4].Length},""${MsgText.Arg[4]}"",]}
   }
/return


Sub Command_CAMPOUT
   /declare MsgText string local ${CommandText}
   /squelch /afollow off
   /squelch /stick off
   /if (${Me.Casting.ID}) {
      /delay 5s !${Me.Casting.ID}
   }
   /if (${Me.Mount.ID}) {
      /dismount
      /delay 1s
   }
   /if (${Me.AltAbilityReady[Companion's Suspension]} && ${Me.Pet.ID} && ${MsgText.Find[sm]}) {
      /casting "Companion's Suspension${PipeChar}alt" -maxtries${PipeChar}5
      /delay 5s !${Me.Pet.ID}
      /bc Msg: ${If[${Me.Pet.ID},Pet didn't cache,Cached the pet]}
   }
   /if (${MsgText.Find[char]}) {
      /bc Msg: Camping to character select
      /docommand /camp
      /delay 3s
      /docommand /endmacro
   }
   /if (${MsgText.Right[-7].Length}>3) {
      /if (${MsgText.Right[-7].Find[trader]}) {
         /bc Msg: Camping and selecting this account's trader to login
         /call Login "${TraderName}"
      } else {
         /bc Msg: Camping and selecting ${MsgText.Right[-7]} to login
         /call Login ${MsgText.Right[-7]}
      }
      /bc Msg: Problem detected. Ending mac
      /delay 3s
      /docommand /endmacro
   }
   /bc Msg: Camping to desktop
   /docommand /camp desktop
   /delay 3s
   /docommand /endmacro
/return


Sub Command_CHECKINV
| /bc if you have an item on yourself or in the bank
   /declare MsgText string local ${CommandText}
   /declare x string ${If[!${MsgText.Arg[2].Equal[NULL]},${MsgText.Arg[2]},]}
   /if (!${x.Length}) /return FALSE
   /bc Msg: ${x} - ${FindItemCount[=${x}]} on me. ${FindItemBankCount[=${x}]} in bank.
/return TRUE


Sub Command_DOAA
   /declare MsgText string local ${CommandText}
   /if (${MsgText.Right[-5].Equal[force]}) {
      /bc Msg: Forcing an AA purchase Check
      /call AAPicker force
   } else {
      /call TFToggle DoAA ${MsgText.Arg[2]}
   }
/return


Sub Command_DOCOMMAND
   /declare MsgText string local ${CommandText}
   /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Processing command ${MsgText.Right[-10]}
   /docommand ${MsgText.Right[-10]}
/return


Sub Command_DOLIST
   /bc Msg: DoBuffs ${DoBuffs}
   /bc Msg: DoDebuffs ${DoDebuffs}
   /bc Msg: DoHeals ${DoHeals}
   /bc Msg: DoEvents ${DoEvents}
   /bc Msg: DoMelee ${DoMelee}
   /bc Msg: DoAfk ${DoAfk}
   /bc Msg: DoSit ${DoSit}
   /bc Msg: DoLoot ${DoLoot}
   /bc Msg: DoForage ${DoForage}
   /bc Msg: DoFW ${DoFW}
/return
      

Sub Command_DOPULL
   /declare MsgText string local ${CommandText}
   /call TFToggle DoPull ${MsgText.Arg[2]}
   /if (${DoPull}) {
      /if (${MsgText.Arg[2].Length} && !${Select[${MsgText.Arg[2]},on,off]}) {
         /varset APPath ${MsgText.Arg[2]}
         /bc Msg: APPath set to [ ${MsgText.Arg[2]} ]
      }
      /if (${TankName.Arg[1,${PipeChar}].Equal[${Me.Name}]}) /varset ACAssistPct 110
      /if (!${CampStatus}) /call Command_MAKECAMP
   }
/return


Sub Command_DOQUEST
   /declare MsgText string local ${CommandText}
   /if (${Select[${MsgText.Arg[2]},on,off,true,false]} || !${MsgText.Arg[2].Length}) {
      /call TFToggle DoQuest ${MsgText.Arg[2]}
      /bc Setting - DoQuest ${DoQuest}
   } else /if (${Cursor.ID} && !${Target.ID} && ${MsgText.Arg[3].Length}) {
      /declare qName string local
      /declare i int local
      /for i 3 to ${Math.Calc[${MsgText.Count[ ]}+1]}
         /varset qName ${qName} ${MsgText.Arg[${i}]}
      /next i
      /ini "${LootIni}" ${Cursor.Name.Left[1]} "${Cursor.Name}" "Quest${PipeChar}${MsgText.Arg[2]}${PipeChar}${qName}${PipeChar}"
      /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Added - "Quest${PipeChar}${MsgText.Arg[2]}${PipeChar}${qName}${PipeChar}" - to loot.ini for ${Cursor.Name}
   }
/return


Sub Command_DROPPET
   /if (${ADCharmID}) {
      /bc Msg: Dropping my pet: ${ADCharmID} - ${Spawn[${ADCharmID}].CleanName}
      /bc IncludeID ${ADCharmID}
      /varset ADDropID ${ADCharmID}
   }
/return


Sub Command_DROPTANK
   /declare MsgText string local ${CommandText}
   /declare tName string local
   /declare tList string local
   /declare x int local 0
   /if (!${MsgText.Arg[2].Length}) {
      /varset tName ${Me.Name}
   } else {
      /varset tName ${MsgText.Arg[2]}
   }   
   /if (${tName.Length}>2 && ${TankList.Find[${tName}]}) {
      /for x 1 to ${Math.Calc[${TankList.Count[ ]}+1]}
         /if (${TankList.Arg[${x}].NotEqual[${tName}]}) /varset tList ${tList}${If[!${tList.Length},, ]}${TankList.Arg[${x}]}
      /next x
      /docommand /mb setinivar TankList "${tList}"
   }
/return


Sub Command_DUCK
   /declare MsgText string local ${CommandText}
   /declare pDuck bool local
   /varset pDuck ${If[(${MsgText.Find[on]} || ${MsgText.Find[true]}),TRUE,${If[(${MsgText.Find[off]} || ${MsgText.Find[false]}),FALSE,${If[${Me.Ducking},FALSE,TRUE]}]}]}
   /if (${pDuck}) {
      /if (!${Me.Ducking}) /nomodkey /keypress duck
      /bc Msg: Ducked
   } else {
      /if (${Me.Ducking}) /nomodkey /keypress duck
      /bc Msg: Un-Ducked
   }
/return


Sub Command_EXCLUDE
   /declare MsgText string local ${CommandText}
   /if (${MsgText.Right[-8].Equal[save]}) {
      /bc Msg: Saving Exclude List
      /call ProcessExcludeList save
   } else {
      /bc Msg: Excluding ${MsgText.Right[-8]}
      /if (!${ExcludeList.Find[${PipeChar}${MsgText.Right[-8]}${PipeChar}]}) /varset ExcludeList ${ExcludeList}${MsgText.Right[-8]}${PipeChar}
      /call ProcessExcludeList
   }
/return


Sub Command_EXCLUDEID
   /declare MsgText string local ${CommandText}
   /declare sID string local ${MsgText.Right[-10]}
   /if (${Spawn[${sID}].ID} && ${Spawn[${sID}].ID}==${sID}) {
      /if (!${ExcludeListID.Find[${PipeChar}${sID}${PipeChar}]}) /varset ExcludeListID ${ExcludeListID}${sID}${PipeChar}
      /squelch /alert add ${AlertList} id ${sID}
   }
/return


Sub Command_FOLLOW(P0)
| "Follow Close" can be called from the Abort command
   /declare MsgText string local ${If[${P0.Length},${P0},${CommandText}]}
   /declare tID int local
   /if (${Select[${MsgText.Arg[2]},NULL,me,close]}) {
      /varset tID ${Spawn[pc =${SenderName}].ID}
   } else {
      /varset tID ${Spawn[pc =${MsgText.Arg[2]}].ID}
   }
   /if (!${tID}) /return
   /squelch /makecamp off
   /varset CampStatus 0
   /varset ACStartHeading 0
   /if (${MoveTo.Moving}) /moveto off
   /call AdvPlugCall ${Spawn[${tID}].CleanName} ${If[${MsgText.Arg[2].Equal[close]} || ${MsgText.Arg[3].Equal[close]},close,]}
   /if (${AdvPath.Following}) /bc Msg: AFing ${Spawn[${FollowID}].CleanName}
/return


Sub Command_GETBANK
   /declare MsgText string local ${CommandText}
   /if (${Cursor.ID}) {
      /call GetBankItem "${Cursor.Name}" ${MsgText.Right[-7]}
   } else /if (${MsgText.Right[-7].Length}) {
      /call GetBankItem ${MsgText.Right[-7]}
   } else {
      /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Usage - Hold Item to get on Cursor or getbank <ItemName> <Quantity> <InStacks> 
   }
/return


Sub Command_GIVE
   /declare MsgText string local ${CommandText}
   /if (${MsgText.Right[-5].Length}) {
      /bc Msg: Giving items to ${MsgText.Arg[2]}
      /call GiveCheck ${MsgText.Right[-5]}
   }
/return


Sub Command_GOTO
   /declare MsgText string local ${CommandText}
   /if (${MsgText.Arg[2].Length} && ${MsgText.Arg[3].Length}) {
      /if (!${Me.Standing}) /stand
      /moveto loc ${MsgText.Arg[2]} ${MsgText.Arg[3]}
   }
/return


Sub Command_GROUPUP
   /declare MsgText string local ${CommandText}
   /declare gName string local ${If[${MsgText.Right[-8].Length},${MsgText.Arg[2]},Normal]}
   /declare IniString string local ${Ini["${IniFile}","Groups",${gName},NOTFOUND]}
   /if (!${IniString.Length} || ${IniString.Equal[NOTFOUND]}) /return
   /declare i int local 0
   /if (${IniString.Arg[1].Equal[${Me.Name}]}) {
      /for i 2 to ${Math.Calc[${IniString.Count[ ]}+1]}
         /if (!${Group.Member[${IniString.Arg[${i}]}].Index}) {
            /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Inviting ${IniString.Arg[${i}]}
            /invite ${IniString.Arg[${i}]}
            /delay 6
         }
      /next i
   }
/return


Sub Command_HAILDELAY
   /declare delaytarget int local ${NetBots[${SenderName}].TargetID}
   /if (!${delaytarget}) {
      /bc Msg: No Target for HailDelay
   } else /if (${Spawn[${delaytarget}].Type.Equal[NPC]} && ${Spawn[${delaytarget}].Distance3D}<25) {
      /declare haildelay int local ${Math.Calc[${Select[${Me.Name},${NetBots.Client.Replace[ ,,]}]}*15+${Math.Rand[2,5]}]}
      /delay ${haildelay}
      /target id ${delaytarget}
      /delay 1s ${Target.ID}==${delaytarget}
      /if (${Target.ID}==${delaytarget}) {
         /hail
         /bc Msg: HailDelay - Hailed
      } else {
         /bc Msg: Targetting Failure for HailDelay
      }
   } else {
      /bc Msg: Invalid Target for HailDelay - ${Spawn[${delaytarget}].Name} (${Spawn[${delaytarget}].Type} / ${Spawn[${delaytarget}].Distance3D})
   }
/return


Sub Command_HAILTARGET
   /declare MsgText string local ${CommandText}
   /declare tTargetID int local ${NetBots[${SenderName}].TargetID}
   /if (!${tTargetID}) {
      /echo ${If[${TimeStampOnEcho},[${Time}] ,]}No Target to Hail
   } else /if (${Spawn[${tTargetID}].Type.Equal[NPC]} && ${Spawn[${tTargetID}].Distance3D}<35) {
      /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Hailing ${SenderName}'s target = ${tTargetID} : ${Spawn[id ${tTargetID}].CleanName}
      /if (${FollowState}) {
         /squelch /afollow pause
      }
      /if (${Me.Class.ShortName.Equal[rog]}) {
         /melee sneak=0 hide=0
         /delay 5
         /if (${Me.Sneaking}) /doability sneak
         |/if (${Me.Hiding}) /doability hide
      }
      /if (${Me.Invis}) {
         /makemevisible
         /delay 1s !${Me.Invis}
      }
      /if (${Me.Invis}) /bc [+r+]Help!![+x+] I'm still [+g+] Invis!! [+x+]
      /target id ${tTargetID}
      /delay 1s ${Target.ID}==${tTargetID}
      /squelch /stick 5
      /delay 5s ${Spawn[id ${tTargetID}].Distance}<15
      /delay 5   
      /hail
      /delay 2
      /squelch /stick off
      /if (${FollowState}) {
         /squelch /afollow unpause
      }
   } else {
      /bc Msg: Invalid Target for HailTarget - ${Spawn[${tTargetID}].Name} (${Spawn[${tTargetID}].Type} / ${Spawn[${tTargetID}].Distance3D})
   }
/return


Sub Command_HANDIN
   /declare MsgText string local ${CommandText}
   /declare tTargetID int local
   /declare cItem string local
   /declare iCount int local 0
   /declare hTotal int local 0
   /if (!${Select[${SenderName},${Me.CleanName},LOCAL]}) {
      /if (${Cursor.ID}) {
         /call ClearCursor force
         /if (${Cursor.ID}) {
            /bc Msg: Can't handin because something is on my cursor and it's not a local command.
            /return
         }
      }
      /squelch /target clear
      /squelch /target id ${NetBots[${SenderName}].TargetID}
      /delay 1s ${Target.ID}==${NetBots[${SenderName}].TargetID}
      /call IsNumber ${MsgText.Arg[2]}
      /if (${Macro.Return.Equal[FALSE]}) {
         /if (${MsgText.Arg[2].Length}) /varset cItem ${MsgText.Arg[2]}
         /if (${MsgText.Arg[3].Length}) /varset hTotal ${MsgText.Arg[3]}
         /if (${MsgText.Arg[4].Length}) /varset iCount ${MsgText.Arg[4]}
         /if (!${hTotal}) /varset hTotal ${FindItemCount[${cItem}]}
      } else {
         /bc Msg: Command format: handin [ItemName] [TotalHandins or 0 for all] [CountPerTrade]
         /return
      }
   } else /if (${Cursor.ID}) {
      /varset cItem ${Cursor.Name}
      /varset hTotal ${FindItemCount[${Cursor.ID}]}
      /if (!${MsgText.Arg[4].Length}) {
         /if (!${MsgText.Arg[3].Length}) {
            /varset iCount ${MsgText.Arg[2]}
         } else { 
            /varset iCount ${MsgText.Arg[3]}
         }
      } else { 
         /varset iCount ${MsgText.Arg[4]}
      }
   }
   /if (!${hTotal}) /varset hTotal ${FindItemCount[${cItem}]}
   /if (!${Target.ID}) {
      /echo ${If[${TimeStampOnEcho},[${Time}] ,]}You must target who to handin to, or use the "give" command.
      /return
   }
   /varset tTargetID ${Target.ID}
   /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Handin [TargetName]:${Target.CleanName} [ItemName]:${cItem} [TotalHandins]:${hTotal}  [CountPerHandin]:${iCount}
   /if (!${tTargetID} || !${cItem.Length} || !${hTotal}>0 ) {
      /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Incorrect usage.  /mb handin "ItemName" or hold item and target handin mob.  then count as only arg..
   } else {
      /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Handing in all ${cItem}'s to ${Target.CleanName}
      /call GiveCheck "${tTargetID}" "${cItem}" ${hTotal} FALSE ${iCount}
   }
/return


Sub Command_HELP
   /declare MsgText string local ${CommandText}
   /if (!${MsgText.Right[-4].Length}) {
      /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Modbot command list.  Use "/mb help command" for specific help on each.
      /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Simple Toggle Commands: ${OnOffCommands}
      /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Other ModBot Commands : ${ActionCommands}
   } else {
      /echo ${If[${TimeStampOnEcho},[${Time}] ,]}This feature is currently under construction.
   }
/return

   
Sub Command_HOLDUP
   /bc Setting - DoBuffs,DoDebuffs,DoEvents,DoMelee,DoCures,DoLoot,DoPull FALSE
   /varset DoDebuffs FALSE
   /varset DoEvents FALSE
   /varset DoHeals TRUE
   /if (!${Me.Class.ShortName.Equal[brd]}) /varset DoBuffs FALSE
   /varset DoMelee FALSE
   /varset DoCures FALSE
   /varset DoPull FALSE
   /varset DoLoot FALSE
/return


Sub Command_INCLUDE
   /declare MsgText string local ${CommandText}
   /bc Msg: Including ${MsgText.Right[-8]}
   /if (${ExcludeList.Find[${MsgText.Right[-8]}]}) {
      /declare sVarM string local ${MsgText.Right[-8]}${PipeChar}
      /if (${ExcludeList.Find[#${MsgText.Right[-8]}]}) /varset sVarM #${sVarM}
      /declare sVarL string local ${ExcludeList.Mid[1,${Math.Calc[${ExcludeList.Find[${sVarM}]}-1]}]}
      /declare sVarR string local ${ExcludeList.Mid[${Math.Calc[${sVarL.Length}+${sVarM.Length}+1]},${ExcludeList.Length}]}
      /varset ExcludeList ${sVarL}${sVarR}
   }
   /call ProcessExcludeList
/return


Sub Command_INCLUDEID
   /declare MsgText string local ${CommandText}
   /declare sID string local ${MsgText.Right[-10]}
   /if (${ExcludeListID.Find[${PipeChar}${sID}${PipeChar}]}) {
      /declare sVarM string local ${sID}${PipeChar}
      /declare sVarL string local ${ExcludeListID.Mid[1,${Math.Calc[${ExcludeListID.Find[${sVarM}]}-1]}]}
      /declare sVarR string local ${ExcludeListID.Mid[${Math.Calc[${sVarL.Length}+${sVarM.Length}+1]},${ExcludeListID.Length}]}
      /varset ExcludeListID ${sVarL}${sVarR}
   }
   /squelch /alert remove ${AlertList} id ${sID}
/return


Sub Command_INVITEALL
   /declare MsgText string local ${CommandText}
   /declare i int local
   /if (${MsgText.Find[ ]}) {
      /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Starting modbot on channel - ${MsgText.Arg[2]}
      /bct ${MsgText.Arg[2]} //mac modbot
   } else {
      /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Starting modbot and inviting all clients.
      /noparse /bca //if (!${Zone.Name.Equal[The Bazaar]}) /mac modbot
   }
   /delay 12s
   /for i 1 to ${NetBots.Counts}
      /if (${Me.GroupSize} && !${Group.Member[0].Leader}) /break
      /if (${Me.GroupSize}>5) /break
      /if (${NetBots.Client.Arg[${i}].NotEqual[${Me.Name}]} && !${Group.Member[${NetBots.Client.Arg[${i}]}].Index}) { 
         /invite ${NetBots.Client.Arg[${i}]}
         /delay ${Math.Calc[${Math.Rand[20]}+10]}
      }
   /next i
/return


Sub Command_LETSROLL
   /declare MsgText string local ${CommandText}
   /bc Setting - DoCures,DoBuffs,DoHeals,DoMelee,DoEvents,DoDebuffs TRUE
   /varset DoDebuffs TRUE
   /varset DoHeals TRUE
   /varset DoMelee TRUE
   /varset DoEvents TRUE
   /varset DoBuffs TRUE
   /varset DoCures TRUE
   /if (${MsgText.Equal[letsroll unpause]}) {
      /bc Setting - MBPause FALSE
      /varset MBPause FALSE
   }
/return


Sub Command_LISTTANKS
   /echo TankList : ${TankList}
/return


Sub Command_LTEACH
| Teach a language. LTeach <name or Number> <#times>
   /declare MsgText string local ${CommandText}
   /declare i int local
   /declare j int local
   /declare x int local
   /declare tLanguage string local ${MsgText.Arg[2]}
   /declare tLanguageID int local
   /declare xMax int local ${MsgText.Arg[3]}
   /declare tLanguageFound bool local FALSE
   /if (!${tLanguage.Length}) {
      /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Specify Language Name or number (from EQ's "/language help" command).
      /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Or use "All" for all your known languages (where you have at least 1 skill point).
      /return FALSE
   }
   /doevents flush Skillup
| The language numbers for EQ's "/language xx" command are based on the Character's available languages (thus, vary from toon to toon)
| As you go through the 'base' list, the EQ number increases by 1 when you encounter a language the toon knows.
| The MQ names/numbers are standard using the entire list (not just the known languages for the toon).
   /for i 1 to 27
      /if (${Me.LanguageSkill[${i}]}) {
         /varcalc j ${j}+1
         /if (${tLanguage.Equal[${j}]} || ${tLanguage.Equal[${Me.Language[${i}]}]} || ${tLanguage.Equal[all]}) {
            /varset tLanguageID ${j}
            /if (${MsgText.Arg[3].Equal[max]} && ${Me.LanguageSkill[${i}]}>=100) {
               /if (${tLanguage.NotEqual[all]}) {
                  /echo ${If[${TimeStampOnEcho},[${Time}] ,]}${Me.Language[${i}]} already Max.
               }
               /continue
            }
            /varset tLanguageFound TRUE
            /language ${tLanguageID}
            /delay 2
            /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Teaching ${Me.Language[${i}]}...
            /if (${MsgText.Arg[3].Equal[max]}) {
               /varset x 0
               /while (${Me.LanguageSkill[${i}]}<100) {
                  /varcalc x ${x}+1
                  /g Maxing ${Me.Language[${i}]} - ${x}
                  /delay 2
                  /doevents Skillup
               }
            } else {
               /if (!${xMax} || ${xMax}>1000) /varset xMax 1000
               /for x 1 to ${xMax}
                  /g Teaching ${Me.Language[${i}]} ${x} of ${xMax}
                  /delay 2
                  /doevents Skillup
               /next x
            }
            /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Finished teaching ${Me.Language[${i}]}
            /if (${tLanguage.NotEqual[all]}) /break
         }
      }
   /next i
   /if (!${tLanguageID}) {
      /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Invalid Language or you do not yet have this language skill.
      /return FALSE
   }
| At end, set language to Common Tongue.
   /if (${tLanguageFound}) {
      /language 1
      /delay 2
   } else /if (${tLanguage.Equal[all]}) {
      /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Already max in all your known languages.
   }
/return TRUE


Sub Command_MAKECAMP
   /squelch /stick off
   /call AdvPlugCall off
   /makecamp on
   /varset MakeCampX ${Me.X}
   /varset MakeCampY ${Me.Y}
   /varset MakeCampD ${Me.Heading.DegreesCCW}
   /varset CampStatus 1
   /bc Msg: Hanging out
/return


Sub Command_MANA
   /bc Msg: ${Me.PctMana}%
/return


Sub Command_MBPAUSE
   /declare MsgText string local ${CommandText}
   /if (${Select[${MsgText.Arg[2]},on,true]} && ${ACState}) {
      /varcalc ACState ${ACState}+2
      /call AdvCombat
   }
   /call TFToggle MBPause ${MsgText.Arg[2]}
   /if (${MBPause}) {
      /if (${CampStatus}) /makecamp off
      /if (${Me.Class.ShortName.Equal[brd]}) {
         /if (${Twist}) /squelch /twist off
      }
   }
/return


Sub Command_MBWAYPLAY
   /declare MsgText string local ${CommandText}
   /call MBWayPlay ${MsgText.Right[-10]}
   /if (${Macro.Return.Equal[false]}) {
      /bc Msg: Problem with playback
   }
/return


Sub Command_MBWAYREC
   /declare MsgText string local ${CommandText}
   /call MBWayRecord ${MsgText.Right[-9]}
/return


Sub Command_PETBACK
   /call BackPets
/return


Sub Command_OFFTANK
   /declare MsgText string local ${CommandText}
   /if (!${TankList.Find[${Me.Name}]}) {
      /echo Not set up as a tank in TankList.
   } else {
      /if (!${TankName.Find[${Me.Name}]}) {
         /call TFToggle OffTank ${MsgText.Arg[2]}
         /if (${OffTank} && !${Melee.AggroMode}) /melee aggro=1
      }
   }
/return


Sub Command_PETTOYS
   /if (${Defined[DoPetToys]}) {
      /if (!${DoPetToys}) /return
      /if (${Spawn[pc ${SenderName}].Pet.ID}) {
         /varset gX ${Me.X.Int}
         /varset gY ${Me.Y.Int}
         /varset gCount 0
         /call PetToys "${Spawn[pc ${SenderName}].Pet.ID}"
         /if (${Macro.Return.NotEqual[TRUE]}) {
            /bc Msg: There seems to be a PetToy problem
         }
         /call GiveCleanup
      } else {
         /bc Msg: No Pet found
      }
   }
/return


Sub Command_PORT
   /declare MsgText string local ${CommandText}
   /declare sPort string local
   /declare x int local
   /if (${MsgText.Arg[2].Equal[list]}) {
      /if (!${Ini.File["${IniFile}"].Section[Ports].Key.Count}) {
         /echo ${If[${TimeStampOnEcho},[${Time}] ,]}No Ports found
      } else {
         /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Saved Ports:
         /for x 1 to ${Ini.File["${IniFile}"].Section[Ports].Key.Count}
            /echo ${If[${TimeStampOnEcho},[${Time}] ,]}> ${Ini.File["${IniFile}"].Section[Ports].Key.KeyAtIndex[${x}]}=${Ini.File["${IniFile}"].Section[Ports].Key.ValueAtIndex[${x}]}
         /next x
      }
      /return
   }   
   /varset sPort ${Ini["${IniFile}","Ports","${MsgText.Arg[2]}"]}
   /if (${sPort.Equal[NULL]}) {
      /if (!${MsgText.Arg[2].Length}) {
         /call Help Port
      } else /if (${Me.Class.PureCaster} && ${MsgText.Arg[2].Equal[gate]}) {
         /bc Msg: Port -Gate- not found.
      } else /if (${Select[${Me.Class.ShortName},DRU,WIZ]}) {
         /bc Msg: Port -${MsgText.Arg[2]}- not found.
      }
      /return
   }
   /if (${MsgText.Arg[2].Equal[evac]}) {
      /stopcast
      /bc Msg: Evacing! - ${sPort.Arg[1,${PipeChar}]}
   } else {
      /if (${Me.Casting.ID} && !${Me.Class.ShortName.Equal[brd]}) {
         /delay 5s !${Me.Casting.ID}
      }
      /bc Msg: Porting - ${sPort.Arg[1,${PipeChar}]}
   }
   /call MQ2Cast "${sPort.Arg[1,${PipeChar}]}" ${sPort.Arg[2,${PipeChar}]} 10s -maxtries${PipeChar}2
/return


Sub Command_RECAST
   /declare MsgText string local ${CommandText}
   /declare b int local 0
   /declare c int local 0
   /for b 1 to ${ABCount}
      /for c 1 to ${Math.Calc[${ABSpellAlias[${b}].Count[${PipeChar}]}+1]}
         /if (${MsgText.Arg[2].Equal[${ABSpellAlias[${b}].Arg[${c},${PipeChar}]}]}) {
            /if (!${ABRecast[${b}]}) {
               /varset ABRecast[${b}] TRUE
               /bc Msg: ${ABSpell[${b}]} will now recast when worn off.
            }
         }
      /next c
   /next b
/return


Sub Command_SAVEGROUP
| Only save if I am Group Leader
   /if (!${Group.Member[0].Leader}) /return
   /declare MsgText string local ${CommandText}
   /declare gName string local ${If[${MsgText.Right[-10].Length},${MsgText.Arg[2]},Normal]}
   /declare gMembers string local ${Me.Name}
   /declare i int local 0
   /for i 1 to ${Group.Members}
      /varset gMembers ${gMembers} ${Group.Member[${i}].Name}
   /next i
   /ini  "${IniFile}" "Groups" "${gName}" "${gMembers}"
   /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Saved Group: "${gName}" - "${gMembers}"
/return


Sub Command_SAYDELAY
   /declare MsgText string local ${CommandText}
   /declare delaytarget int local ${NetBots[${SenderName}].TargetID}
   /if (!${delaytarget}) {
      /bc Msg: No Target for SayDelay
   } else /if (!${MsgText.Right[-9].Length}) {
      /bc Msg: No Text for SayDelay
   } else /if (${Spawn[${delaytarget}].Type.Equal[NPC]} && ${Spawn[${delaytarget}].Distance3D}<25) {
      /declare saydelay int local ${Math.Calc[${Select[${Me.Name},${NetBots.Client.Replace[ ,,]}]}*15+${Math.Rand[2,5]}]}
      /delay ${saydelay}
      /target id ${delaytarget}
      /delay 1s ${Target.ID}==${delaytarget}
      /if (${Target.ID}==${delaytarget}) {
         /say ${MsgText.Right[-9]}
         /bc Msg: SayDelay - ${MsgText.Right[-9]}
      } else {
         /bc Msg: Targetting Failure for SayDelay
      }
   } else {
      /bc Msg: Invalid Target for SayDelay - ${Spawn[${delaytarget}].Name} (${Spawn[${delaytarget}].Type} / ${Spawn[${delaytarget}].Distance3D})
   }
/return


Sub Command_SAYTARGET
   /declare MsgText string local ${CommandText}
   /if (!${Spawn[=${SenderName}].ID}) /return
   /if (!${NetBots[${SenderName}].TargetID}) /return
   /declare tTarget int local ${NetBots[${SenderName}].TargetID}
   /if (${FollowState}) {
      /squelch /afollow pause
   }
   /if (${Me.Invis}) /makemevisible
   /target id ${tTarget}
   /delay 1s ${Target.ID}==${tTarget}
   /if (${Me.Invis}) /bc Msg: [+r+]Help!![+x+] I'm still [+g+] Invis!! [+x+]
   /squelch /stick 5
   /delay 5s ${Spawn[id ${tTarget}].Distance}<15  
   /delay 2
   /say ${MsgText.Right[-10]}
   /delay 2
   /squelch /stick off
   /if (${FollowState}) {
      /squelch /afollow unpause
   }
/return


Sub Command_SCRIPT
  /declare MsgText string local ${CommandText}
  /call MBScript ${MsgText.Right[-7]}
/return


Sub Command_SEARCH
   /declare MsgText string local ${CommandText}
   /declare SearchText string local ${MsgText.Right[-7]}
   /declare ReplyText string local ${FindItemCount[${SearchText}]} on me and ${FindItemBankCount[${SearchText}]} in my bank. 
   /declare Count1 int local ${FindItemCount[${SearchText}]}
   /declare BCount1 int local ${FindItemBankCount[${SearchText}]}
   /if (${Count1}>0 && ${FindItemCount[=${SearchText}]}!=${Count1} || ${BCount1}>0 && ${FindItemBankCount[=${SearchText}]}!=${BCount1}) {
      /varset ReplyText ${ReplyText} - Probably different items (generic search matches).
   }
   /bc ${ReplyText}
/return


Sub Command_SELL
   /declare MsgText string local ${CommandText}
   /declare MerchName string local
   /if (!${MsgText.Right[-5].Length} && ${Target.Class.Name.Equal[Merchant]}) {
      /varset MerchName ${Target.CleanName}
   } else /if (${MsgText.Right[-5].Length} && !${Category.Find[${MsgText.Right[-5]}]}) {
      /varset MerchName ${MsgText.Right[-5]}
   } else /if (${Spawn[Merchant].Distance} < 20) {
      /varset MerchName ${Spawn[Merchant].CleanName}
   }
   /if (!${MerchName.Length}) {
      /bc Msg: Unable to find merchant. Exiting Sell.
	  /return
   }
   /bc Msg: Selling items to ${MerchName}
   /call SellCheck "${MerchName}" ${If[${MsgText.Length}>5,${MsgText.Right[-5]},]}
   /bc Msg: Selling completed
/return


Sub Command_SETINIVAR
   /declare MsgText string local ${CommandText}
   /if (${MBDebug}) {
      /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Macro.RunTime:${Macro.RunTime} LoadIniVar - MacroVar == ${MacroVar}  File : ${MyIni} VarType: ${VarType}
   }
   /if (!${MsgText.Arg[2].Length} || !${MsgText.Arg[3].Length}) {
      /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Usage: /mb setinivar <Varname> <Setting>  (use quotes around multiple word settings)
      /return
   }
   /declare IniSection string local
   /declare a int local
   /declare b int local
   /declare IniVar string local ${MsgText.Arg[2]}
   /declare IniSetting string local ${MsgText.Arg[3]}
   /declare VarNumber int local
   /declare tSection string local Settings Melee AdvHeal AdvDebuff AdvBuff AdvEvent AdvCure AdvPull AdvCure
   /declare tClass string local
   /declare tVar string local
   /declare tNumCnt int local
   /if (${IniVar.Right[1].Equal[]]} && ${IniVar.Find[[]}) {
      /varset VarNumber ${MsgText.Arg[2].Arg[2,[].Left[-1]}
      /varset IniVar ${MsgText.Arg[2].Arg[1,[]}${VarNumber}
   }
   /if (${MsgText.Arg[4].Length}) {
      /for b 4 to ${Math.Calc[${MsgText.Count[ ]}+1]}
         /varset IniSetting ${IniSetting} ${MsgText.Arg[${b}]}
      /next b
   }
   /for a 1 to 9
      /varset IniSection ${tSection.Arg[${a}]}
      /if (!${IniSection.Length}) /return
      /echo ${If[${TimeStampOnEcho},[${Time}] ,]}${IniFile},${IniSection}
      /for b 1 to ${Math.Calc[${Ini["${IniFile}",${IniSection}].Count[${PipeChar}]}+1]}
         /if (${Ini["${IniFile}",${IniSection}].Arg[${b},${PipeChar}].Equal[${IniVar}]}) {
            /ini "${IniFile}" "${IniSection}" "${IniVar}" "${IniSetting}"
            /echo ${If[${TimeStampOnEcho},[${Time}] ,]}You're setting for ${MsgText.Arg[2]} is now ${Ini["${IniFile}",${IniSection},${IniVar}]} in your .ini file.
            /if (${Defined[${MsgText.Arg[2]}]}) {
               /varset ${MsgText.Arg[2]} ${IniSetting}
            } else {
               /varset IniVar ${MsgText.Arg[2].Arg[1,[]}
               /if (!${Defined[${IniVar}]}) {
                  /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Cannot set variable in memory.  Defined variable does not match. -Restart required to set it.
                  /return
               }
               /varset ${MsgText.Arg[2]} ${IniSetting}
            }
            /return
         }
      /next b
   /next a
   /varset tClass ${IniVar.Left[2]}
   /if (!${Select[${tClass},AH,AD,AB,AE,AQ]}) /return
   /if (${Select[${tClass},AH,AD,AB,AE,AQ]} && ${${tClass}NewFormat} && ${Select[${IniVar.Right[1]},1,2,3,4,5,6,7,8,9,0]}) {
      /varset tNumCnt 1
:IniNextNum
      /varcalc a ${IniVar.Length}-${tNumCnt}
      /if (${Select[${IniVar.Mid[${a},1]},1,2,3,4,5,6,7,8,9,0]}) {
         /varcalc tNumCnt ${tNumCnt}+1
         /goto :IniNextNum
      }
      /varset tVar ${IniVar.Left[-${tNumCnt}]}
      /varset tVar ${tVar.Right[-2]}
      /varset IniSection ${tClass}${IniVar.Right[${tNumCnt}]}
      /echo ${If[${TimeStampOnEcho},[${Time}] ,]}${IniFile},${IniSection}
      /for b 1 to ${Math.Calc[${Ini["${IniFile}",${IniSection}].Count[${PipeChar}]}+1]}
         /if (${Ini["${IniFile}",${IniSection}].Arg[${b},${PipeChar}].Equal[${tVar}]}) {
            /ini "${IniFile}" "${IniSection}" "${tVar}" "${IniSetting}"
            /echo ${If[${TimeStampOnEcho},[${Time}] ,]}You're setting for ${MsgText.Arg[2]} is now ${Ini["${IniFile}",${IniSection},${tVar}]} in your .ini file.
            /if (${Defined[${MsgText.Arg[2]}]}) {
               /varset ${MsgText.Arg[2]} ${IniSetting}
            } else {
               /varset IniVar ${MsgText.Arg[2].Arg[1,[]}
               /if (!${Defined[${IniVar}]}) {
                  /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Cannot set variable in memory.  Defined variable does not match. -Restart required to set it.
                  /return
               }
               /varset ${MsgText.Arg[2]} ${IniSetting}
            }
            /return
         }
      /next b
   }   
   /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Variable not found
/return


Sub Command_SETLOOT
   /declare MsgText string local ${CommandText}
   /if (!${MsgText.Right[-7].Length} || !${Cursor.ID}) {
      /call Help SetLoot
      /return
   }
   /if (${Ini["${LootIni}","${Cursor.Name.Left[1]}","${Cursor.Name}"].Equal[KEEP]}) /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Item WAS set to KEEP in loot file.
   /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Setting item: ${Cursor.Name} to [${MsgText.Right[-8]}] in your loot file. Length of ${MsgText.Right[-8].Length}.
   /ini "${LootIni}" "${Cursor.Name.Left[1]}" "${Cursor.Name}" ${MsgText.Right[-8]}
/return


Sub Command_SETPULLER
   /declare MsgText string local ${CommandText}
   /declare s string local ${MsgText.Right[-10]}
   /declare tTargetID int local ${NetBots[${SenderName}].TargetID}
   /declare tPullerName string local
   /if (${s.Length}) {
      /varset s ${s.Left[1].Upper}${s.Right[-1]}
      /if (${Spawn[pc =${s.Arg[1,${PipeChar}]}].ID}) {
         /varset tPullerName ${Spawn[pc =${s.Arg[1,${PipeChar}]}].CleanName}
      } else /if (${NetBots[${s.Arg[1,${PipeChar}]}].ID}) {
         /varset tPullerName ${s.Arg[1,${PipeChar}]}
      } else /if (${Spawn[pcpet =${s}].ID}) {
         /bc Msg: Pets cannot be puller
      } else /if (${Spawn[mercenary =${s}].ID}) {
         /bc Msg: Mercenaries cannot be puller
      } else {
         /varset tPullerName ${s}
      }
   } else /if (${tTargetID}) {
      /if (${Spawn[pc id ${tTargetID}].ID}) {
         /varset tPullerName ${Spawn[pc id ${tTargetID}].CleanName}
      } else /if (${Spawn[pcpet id ${tTargetID}].ID}) {
         /bc Msg: Pets cannot be puller
      } else /if (${Spawn[mercenary id ${tTargetID}].ID}) {
         /bc Msg: Mercenaries cannot be puller
      }
   }
   /if (${tPullerName.Length}) {
      /varset PullerName ${tPullerName}   
      /if (${ManualPuller}) {
         /bc Msg: ${PullerName}${If[!${Spawn[pc =${PullerName}].ID}, (NOT in Zone),]} is now PULLER
      } else {
         /varset ManualPuller TRUE
         /bc Msg: ${PullerName}${If[!${Spawn[pc =${PullerName}].ID}, (NOT in Zone),]} is now PULLER (Ignoring Group Puller Role)
      }
   } else {
      /bc Msg: Clearing/Resetting Puller (Group Roles will be considered)
      /varset ManualPuller FALSE
      /varset PullerName
      /call SetPuller
      /if (!${PullerName.Length}) {
         /bc Msg: NO Puller Assigned
      }
   }
/return


Sub Command_SETTANK
   /declare MsgText string local ${CommandText}
   /declare s string local ${MsgText.Right[-8]}
   /declare tTargetID int local ${NetBots[${SenderName}].TargetID}
   /declare tTankName string local
   /if (${s.Length}) {
      /varset s ${s.Left[1].Upper}${s.Right[-1]}
      /if (${Spawn[pc =${s.Arg[1,${PipeChar}]}].ID}) {
         /varset tTankName ${Spawn[pc =${s.Arg[1,${PipeChar}]}].CleanName}${If[!${s.Arg[2,${PipeChar}].Equal[pet]},,${PipeChar}pet]}
      } else /if (${NetBots[${s.Arg[1,${PipeChar}]}].ID}) {
         /varset tTankName ${s.Arg[1,${PipeChar}]}${If[!${s.Arg[2,${PipeChar}].Equal[pet]},,${PipeChar}pet]}
      } else /if (${Spawn[pcpet =${s}].ID}) {
         /bc Msg: For pet tank, use "<player>${PipeChar}pet" or target pet and use SetTank command without parms
      } else /if (${SpawnCount[mercenary =${s}]}==1) {
         /varset tTankName ${Spawn[mercenary =${s}].CleanName}
      } else /if (${SpawnCount[mercenary =${s}]}>1) {
         /bc Msg: Duplicate Merc Names found. Target merc and use settank command without parms
      } else {
         /varset tTankName ${s}
      }
   } else /if (${tTargetID}) {
      /if (${Spawn[pc id ${tTargetID}].ID}) {
         /varset tTankName ${Spawn[pc id ${tTargetID}].CleanName}
      } else /if (${Spawn[pcpet id ${tTargetID}].ID}) {
         /varset tTankName ${Spawn[pcpet id ${tTargetID}].Master.CleanName}${PipeChar}pet
      } else /if (${Spawn[mercenary id ${tTargetID}].ID}) {
         /varset tTankName ${Spawn[mercenary id ${tTargetID}].CleanName}
      }
   }
   /if (${tTankName.Length}) {
      /varset TankName ${tTankName}
      /call SetTankID
      /if (${ManualTank}) {
         /bc Msg: ${TankName.Arg[1,${PipeChar}]}${If[!${TankName.Arg[2,${PipeChar}].Equal[pet]},,'s Pet]}${If[!${TankID}, (NOT in Zone),]} is now TANK
      } else {
         /varset ManualTank TRUE
         /bc Msg: ${TankName.Arg[1,${PipeChar}]}${If[!${TankName.Arg[2,${PipeChar}].Equal[pet]},,'s Pet]}${If[!${TankID}, (NOT in Zone),]} is now TANK (Ignoring Group MA/MT Roles)
      }
   } else {
      /bc Msg: Clearing/Resetting Tank (Group Roles will be considered)
      /varset ManualTank FALSE
      /varset TankName
      /call SetTank
      /if (!${TankName.Length}) {
         /bc Msg: NO Tank Assigned
      }
   }
/return


Sub Command_SETTANKS
   /bca setinivar TankList "${TankList}"
/return


Sub Command_SINGLEFILE
| Tell each NetBot member to Follow the Member "above" him
   /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Single file following in progress.
   /if (${SenderName.NotEqual[${Me.CleanName}]}) /return
   /declare i int local 0
   /declare CurBot string local ${Me.CleanName}
   /for i 1 to ${NetBots.Counts}
      /if (${NetBots.Client.Arg[${i}].NotEqual[${CurBot}]} && ${NetBots.Client.Arg[${i}].NotEqual[${Me.CleanName}]}) {
         /if (${Spawn[pc =${NetBots.Client.Arg[${i}]}].ID}) {
            /bct ${NetBots.Client.Arg[${i}]} follow ${CurBot}
            /varset CurBot ${NetBots.Client.Arg[${i}]}
         }
      }
      /delay 2
   /next i
/return


Sub Command_STOP
   /if (${Select[${ACState},2,4]}) {
      /varset ACState 1
   }
   /call AdvPlugCall off
   /squelch /stick off
   /if (${CampStatus}==1) {
      /varset MakeCampX ${Me.X}
      /varset MakeCampY ${Me.Y}
      /varset MakeCampD ${Me.Heading.DegreesCCW}
      /makecamp on
   } else {
      /makecamp off
   }
   /varset ACStartY ${Me.Y}
   /varset ACStartX ${Me.X}
   /varset ACStartHeading ${Me.Heading.DegreesCCW}
   /bc Msg: No longer on AF
/return


Sub Command_SWAPITEM(P0)
| "SwapItem zone" can be called from DelayOnZone
   /declare MsgText string local ${If[${P0.Length},${P0},${CommandText}]}
   /declare sName string local
   /declare sSlots string local
   /declare sItems string local
   /declare i int local 0
   /declare x int local
   /declare xSlot string local
   /declare xItem string local
   /if (!${MsgText.Arg[2].Length}) {
      /call Help SwapItem
      /return
   }
   /varset sName ${If[${MsgText.Arg[2].Equal[zone]},${Zone.ShortName},${MsgText.Arg[2]}]}
   /varset sItems ${Ini["${IniFile}","ItemSwaps","${sName}"]}
   /if (${sItems.Equal[NULL]}) {
      /if (${sName.NotEqual[${Zone.ShortName}]}) {
         /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Saved Swap Set not found: ${sName}. Please use <swapsave> command first.
      }
      /return
   }
   /varset sSlots ${If[${MsgText.Arg[3].Length},${MsgText.Arg[3]},${slotNames}]}
   /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Checking for Item Swaps in Swap Set: ${sName}
   /for x 1 to ${Math.Calc[${sItems.Count[,]} + 1]}
      /varset xSlot ${sItems.Arg[${x},,].Arg[2,${PipeChar}]}
      /if (${Select[${xSlot},${sSlots}]}) {
         /varset xItem ${sItems.Arg[${x},,].Arg[1,${PipeChar}].Replace[ ,]}
| This is also how EQ's Bandolier functions. If you have an empty bandolier slot, it unequips on swap.
         /if (${xItem.Equal[NULL]}) {
            /call UnEquipItem "${xSlot}"
         } else {
            /call EquipItem "${xItem}" "${xSlot}"
         }
         /if (${Macro.Return.Length}) /varcalc i ${i}+1
      }
   /next x
   /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Completed Item Swaps${If[!${i}, (No swaps necessary),]}.
/return


Sub Command_SWAPSAVE
   /declare MsgText string local ${CommandText}
   /declare sName string local
   /declare sItems string local
   /declare x int local
   /declare xSlot string local
   /declare xItem string local
   /if (${MsgText.Arg[2].Equal[list]}) {
      /varset sName ${Ini["${IniFile}","ItemSwaps"]}
      /if (${sName.Equal[NULL]}) {
         /echo ${If[${TimeStampOnEcho},[${Time}] ,]}No Saved Swap Sets found
      } else /if (!${MsgText.Arg[3].Length}) {
         /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Saved Swap Sets:
         /for x 1 to ${Math.Calc[${sName.Count[${PipeChar}]} - 1]}
            /echo ${If[${TimeStampOnEcho},[${Time}] ,]}${sName.Arg[${x},${PipeChar}]}
         /next x
      } else {
         /varset sItems ${Ini["${IniFile}","ItemSwaps","${MsgText.Arg[3]}"]}
         /if (${sItems.Equal[NULL]}) {
            /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Saved Swap Set not found: ${MsgText.Arg[3]}
         } else {
            /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Items for Saved Swap Set: ${MsgText.Arg[3]}
            /for x 1 to ${Math.Calc[${sItems.Count[,]} + 1]}
               /echo ${If[${TimeStampOnEcho},[${Time}] ,]}> ${sItems.Arg[${x},,].Arg[2,${PipeChar}]} - ${sItems.Arg[${x},,].Arg[1,${PipeChar}].Replace[ ,]} (${FindItem[${sItems.Arg[${x},,].Arg[1,${PipeChar}].Replace[ ,]}].Name})
            /next x
         }
      }
      /return
   }
   /varset sName ${If[${MsgText.Arg[2].Equal[zone]},${Zone.ShortName},${MsgText.Arg[2]}]}
   /if (${sName.Equal[normal]} || ${MsgText.Arg[3].Equal[all]}) {
      /for x 1 to ${numSlots}
         /varset sItems ${sItems}${If[!${sItems.Length},,, ]}${Me.Inventory[${slotNames.Arg[${x},,]}].ID}${PipeChar}${slotNames.Arg[${x},,]}
      /next x
   } else /if (!${sName.Length} || !${MsgText.Arg[3].Length}) {
      /call Help SwapSave
      /return
   } else {
      /for x 1 to ${Math.Calc[${MsgText.Arg[3].Count[,]} + 1]}
         /if (${MsgText.Arg[3].Arg[${x},,].Find[${PipeChar}]}) {
            /varset xItem ${MsgText.Arg[3].Arg[${x},,].Arg[1,${PipeChar}]}
            /varset xSlot ${MsgText.Arg[3].Arg[${x},,].Arg[2,${PipeChar}]}
         } else {
            /varset xItem
            /varset xSlot ${MsgText.Arg[3].Arg[${x},,]}
         }
         /if (!${Select[${xSlot},${slotNames}]}) {
            /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Invalid Slot: ${xSlot} - skipping
            /continue
         } else /if (!${xItem.Length}) {
            /varset xItem ${Me.Inventory[${slotNames.Arg[${Select[${xSlot},${slotNames}]},,]}].ID}
            /varset xSlot ${slotNames.Arg[${Select[${xSlot},${slotNames}]},,]}
         } else {
            /if (!${FindItem[${xItem}].ID}) {
               /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Warning - Item not found: ${xItem}
            }
            /varset xItem ${FindItem[${xItem}].ID}
            /varset xSlot ${slotNames.Arg[${Select[${xSlot},${slotNames}]},,]}
            /if (!${FindItem[${xItem}].WornSlot[${xSlot}]}) {
               /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Invalid Item/Slot Combination - ${MsgText.Arg[3]} - skipping
               /continue
            }
         }
         /varset sItems ${sItems}${If[!${sItems.Length},,, ]}${xItem}${PipeChar}${xSlot}
      /next x
   }
   /if (${sName.Length} && ${sItems.Length}) {
      /ini "${IniFile}" "ItemSwaps" "${sName}" "${sItems}"
      /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Saved Swap Set: ${sName}
      /for x 1 to ${Math.Calc[${sItems.Count[,]} + 1]}
          /echo ${If[${TimeStampOnEcho},[${Time}] ,]}> ${sItems.Arg[${x},,].Arg[2,${PipeChar}]} - ${sItems.Arg[${x},,].Arg[1,${PipeChar}].Replace[ ,]} (${FindItem[${sItems.Arg[${x},,].Arg[1,${PipeChar}].Replace[ ,]}].Name})
      /next x
   }
/return


Sub Command_SWITCH
   /declare MsgText string local ${CommandText}
   /varset ACSwitchID 0
   /if (${OffTank}) /return
   /if (!${MsgText.Arg[2].Length}) {
      /if (${NetBots[${SenderName}].ID}) {
         /varset ACSwitchID ${NetBots[${SenderName}].TargetID} 
      } else /if (${NetBots[${PullerName}].ID}) {
         /varset ACSwitchID ${NetBots[${PullerName}].TargetID}
      } else /if (${Spawn[${PullerName}].ID}) {
         /call GetTargetID ${PullerName}
         /varset ACSwitchID ${Macro.Return}
      }
      /if (!${ACSwitchID}) {
         /echo ${If[${TimeStampOnEcho},[${Time}] ,]}No Switch Target or Sender/Puller Target
         /return
      }
      /if (!${Spawn[${ACSwitchID}].ID}) {
         /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Switch Spawn not found
         /return
      }
   } else {
      /if (${Spawn[npc radius 100 id ${MsgText.Arg[2]}].ID}) {
         /varset ACSwitchID ${Spawn[${MsgText.Arg[2]}].ID}
      } else {
         /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Switch Spawn not found
         /return
      }
   }
   /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Switching to ${Spawn[${ACSwitchID}]} (#${ACSwitchID})
|  /varset ACState 100
   /varset DoMelee TRUE
   /varset ACMATarget ${ACSwitchID}
   /squelch /melee reset
   /call BackPets
   /target id ${ACMATarget}
   /delay 2s ${Target.ID}==${ACMATarget}
|  /if (${Spawn[${ACMATarget}].Distance3D} > ${ACLeash}) /return
   /if (${ACManaPct}!=101) {
      /stick id ${ACMATarget}
      /attack on
   }
   /call SendPets
/return


Sub Command_TANKANDHEALS
   /declare MsgText string local ${CommandText}
   /if (${TankName.Arg[1,${PipeChar}].Equal[${Me.Name}]}) {
      /varset DoMelee TRUE
      /varset DoDebuffs TRUE
      /varset DoEvents TRUE
   } else {
      /varset DoMelee FALSE
      /if (${ACState}) {
         /varcalc ACState ${ACState}+2
         /call AdvCombat
      }
      /squelch /melee reset
      /varset ACMATarget 0
      /call BackPets
      /varset DoDebuffs FALSE
      /varset DoEvents FALSE
   }
   /varset DoHeals TRUE
   /varset DoCures TRUE
   /varset DoBuffs TRUE
   /bc Msg: Setting Tank-only Melee/Debuffs. Heals/Cures/Buffs from others.
   /if (${MsgText.Equal[tankandheals unpause]}) {
      /bc Setting - MBPause FALSE
      /varset MBPause FALSE
   }
/return


Sub Command_XBOTS
| Fill the XTargets (bottom-up) with the other characters on NetBots (not in my group), auto-hate the rest.
   /declare b int local
   /declare x int local
   /if (!${NetBots.Counts} || !${Me.XTargetSlots}) /return
   /varset x ${Me.XTargetSlots}
   /for b 1 to ${NetBots.Counts}
      /if (${x}<1) /return
      /if (!${NetBots.Client[${b}].Equal[${Me.CleanName}]} && !${NetBots[${NetBots.Client[${b}]}].InGroup}) {
         /docommand /xtarget set ${x} ${NetBots.Client[${b}]}
         /varcalc x ${x}-1
      }
   /next b
   /if (${x}<1) /return
   /for b ${x} downto 1
      /docommand /xtarget set ${b} AH
   /next b
/return



Sub EquipItem(sItem,sSlot)
   /declare oldItemSlot string local
   /if (${sItem.Equal[NULL]}) /return
   /if (${sSlot.Equal[NULL]}) {
      /varset sSlot ${FindItem[${sItemName}].WornSlot[1].Name}
   }
   /if (!${Select[${sSlot},${slotNames}]}) {
      /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Invalid Equipment Slot: ${sSlot} - skipping
      /return
   }
   /if (!${FindItem[${sItem}].ID}) {
      /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Item not found: ${sItem}${PipeChar}${sSlot} - skipping
      /return
   }
   /varset sItem ${FindItem[${sItem}].ID}
   /if (${Me.Inventory[${sSlot}].ID}==${sItem}) /return
   /if (!${FindItem[${sItem}].WornSlot[${sSlot}]}) {
      /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Invalid Item/Slot Combination - ${sItem}${PipeChar}${sSlot} - skipping
      /return
   }
   /if (${sSlot.Equal[mainhand]}) {
      /if (${FindItem[${sItem}].Type.Left[2].Equal[2H]} && ${Me.Inventory[offhand].ID}) {
         /varset oldItemSlot ${Me.Inventory[offhand].ID}${PipeChar}offhand
         /call UnEquipItem offhand
      }
   } else /if (${sSlot.Equal[offhand]}) {
      /if (${Me.Inventory[mainhand].Type.Left[2].Equal[2H]}) {
         /varset oldItemSlot ${Me.Inventory[mainhand].ID}${PipeChar}mainhand
         /call UnEquipItem mainhand
      }
   }
   /varset oldItemSlot ${If[!${oldItemSlot.Length},,${oldItemSlot},]}${If[${Me.Inventory[${sSlot}].ID},${Me.Inventory[${sSlot}].ID},EMPTY]}${PipeChar}${sSlot}
   /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Swapping (${FindItem[${sItem}].Name}) to ${sSlot}
   /call SwapItem "${sItem}" "${sSlot}"
/return ${oldItemSlot}


Sub SwapItem(sItem,sSlot)
   /if (${Cursor.ID}) /call ClearCursor force
   /squelch /exchange "${sItem}" ${sSlot}
   /delay 1
/return


Sub UnEquipItem(sSlot)
   /declare oldItemSlot string local
   /if (${sSlot.Equal[NULL]}) /return
   /if (!${Select[${sSlot},${slotNames}]}) {
      /echo ${If[${TimeStampOnEcho},[${Time}] ,]}UnEquip - Invalid Slot: ${sSlot}
      /return
   }
   /if (${Me.Inventory[${sSlot}].ID}) {
      /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Unequipping (${Me.Inventory[${sSlot}].Name}) from ${sSlot}
      /varset oldItemSlot ${If[${Me.Inventory[${sSlot}].ID},${Me.Inventory[${sSlot}].ID},EMPTY]}${PipeChar}${sSlot}
      /squelch /unequip ${sSlot}
      /delay 1
   }
/return ${oldItemSlot}


Sub Help(string Command)
   /if (!${Command.Length}) {
      /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Help unavailable at this time. Please see ModBot wiki.
   } else /if (${Command.Equal[AddBuff]}) {
      /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Format: /mb addbuff "<Spell Name>${PipeChar}<gem,alt,item,ect>" <Alias> <TarClass> <TarCnt>
   } else /if (${Command.Equal[AddDebuff]}) {
      /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Format: /mb adddebuff "<Spell Name>${PipeChar}<gem,alt,item,ect>" <Alias> <TarType> <TarCnt>
   } else /if (${Command.Equal[HailTarget]}) {
      /echo This command attempts to drop invis, get close to the target, and hail.
   } else /if (${Command.Equal[Search]}) {
      /echo Format: /mb search <itemName>.  "/mb search Adventurer's stone" will tell you if it's in your bank or inventory, or maybe you just don't have one.
   } else /if (${Command.Equal[SetLoot]}) {
      /echo SetLoot Usage - Hold item on cursor.  Use: setloot <loottype> 
   } else /if (${Command.Equal[SwapItem]}) {
      /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Usage: /mb swapitem [swapname] [slot,slot,...] 
      /echo ${If[${TimeStampOnEcho},[${Time}] ,]}  This will load "swapname" from "ItemSwaps" section of ini file and equip items for you.
      /echo ${If[${TimeStampOnEcho},[${Time}] ,]}  Adding the [slot] will limit to the slot(s) specified.
   } else /if (${Command.Equal[SwapSave]}) {
      /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Usage: /mb swapsave <Normal or Zone or SwapName> <all or <item${PipeChar}>location,<item${PipeChar}>location,...>
      /echo ${If[${TimeStampOnEcho},[${Time}] ,]}   Ex: /mb swapsave Uqua "Club of Destructive Protection${PipeChar}ranged"
      /echo ${If[${TimeStampOnEcho},[${Time}] ,]}   Or: /mb swapsave Uqua 67739${PipeChar}ranged
      /echo ${If[${TimeStampOnEcho},[${Time}] ,]}   Or: /mb swapsave Uqua ranged,primary (using whatever is currently in ranged and primary slots)
      /echo ${If[${TimeStampOnEcho},[${Time}] ,]}   Or: /mb swapsave Uqua null${PipeChar}ranged will save Ranged as Empty (to force Unequip on subsequent SWAPITEMS)
      /echo ${If[${TimeStampOnEcho},[${Time}] ,]}If you use 'Normal', 'all' is implied. Using 'zone' will save using the zone shortname. Using the zone name
      /echo ${If[${TimeStampOnEcho},[${Time}] ,]}will also cause automatic swapping when zoning into this zone. Remember to reset when you exit.
   } else {
      /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Help unavailable at this time. Please see ModBot wiki.
   }
/return


Sub ResetOutDoors
   /doevents flush OUTDOORS
   /varset OutDoorsFail FALSE
/return


Sub Event_OUTDOORS(string Line)
   /declare a int local
   /declare tTwistGems string local
|   /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Outdoor Spell...
   /varset OutDoorsFail TRUE
   /if (${Me.Class.ShortName.Equal[brd]} && ${Line.Find[levita]}) {
|/echo ${Twist.Current} - ${Me.Gem[${Twist.Current}]} - Levitate trigger
      /varset tTwistGems ${Twist.List}
      /for a 1 to ${ABCount}
         /if (${Spell[${ABSpellID[${a}]}].HasSPA[57]}) {
            /call AddToRestrictedList ${Spell[${ABSpellID[${a}]}].ID}
            /if (${Select[${ABGem[${a}]},${ValidSpellGems}]}) {
               /if (${Me.Gem[${ABGem[${a}]}].ID}==${Spell[${ABSpellID[${a}]}].ID}) {
                  /call EditTwist "${tTwistGems}" ${ABGem[${a}]} delete
                  /varset tTwistGems ${Macro.Return}
                  /call EditTwist "${BardCombatGems}" ${ABGem[${a}]} delete
                  /varset BardCombatGems ${Macro.Return}
                  /call EditTwist "${BardRestGems}" ${ABGem[${a}]} delete
                  /varset BardRestGems ${Macro.Return}
               }
            }
         }
      /next a 
      /if (${Twist.Twisting}) {
         /if (!${Twist.List.Equal[${tTwistGems}]}) {
            /if (${tTwistGems.Length}) {
               /squelch /twist ${tTwistGems}
            } else {
               /squelch /twist clear
            }
         }
      }
      /call BuildRestrictedList
|     /if (!${ACState}) { 
|        /varset BardRestGems
|        /varset BardCombatGems
|     }
|     /call ABBuffCheck
      /doevents flush OUTDOORS
   }
/return


Sub AddToRestrictedList(int tSpellID)
   /declare tString1 string local
   /declare tString3 string local
   /if (!${Spell[${tSpellID}].ID}) /return
   /bc Msg: ${If[${Spell[${tSpellID}].Location}==1,Outdoor only,Restricted]} spell. Turning off spell [${Spell[${tSpellID}].Name}] for this zone.
   /echo ${If[${TimeStampOnEcho},[${Time}] ,]}${If[${Spell[${tSpellID}].Location}==1,Outdoor only,Restricted]} spell. Turning off spell [${Spell[${tSpellID}].Name}] for this zone.
   /varset tString1 ${Ini["${MBCommon}",RestrictedList,"${Zone.ShortName}","NOTFOUND"]}
   /if (!${tString1.Length} || ${tString1.Equal[NOTFOUND]}) {
      /varset tString3 ${Zone.Name},${Spell[${tSpellID}].Name}${PipeChar}${tSpellID}
   } else {
      /if (${tString1.Find[${tSpellID}]}) /return FALSE
      /varset tString3 ${tString1},${Spell[${tSpellID}].Name}${PipeChar}${tSpellID}
   }
   /if (${tString3.NotEqual[${tString1}]}) {
      /ini "${MBCommon}" "RestrictedList" "${Zone.ShortName}" "${tString3}"
      /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Added "${Spell[${tSpellID}].Name}${PipeChar}${tSpellID}" to the Restricted list for ${Zone.ShortName}=${Zone.Name}
      /return TRUE
   }
/return FALSE
               

Sub LoadRestrictedSpells
   /declare a int local
   /call BuildRestrictedList
|/echo LRS - ${SpellsRestrictedByZone}
   /if (${SpellsRestrictedByZone.Length}) {
      /for a 1 to ${Math.Calc[${SpellsRestrictedByZone.Count[,]}+1]}
         /if (${Spell[${SpellsRestrictedByZone.Arg[${a},,]}].SpellType.Find[Beneficial]}) {
            /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Zone Restricted Buff - ${Spell[${SpellsRestrictedByZone.Arg[${a},,]}]} : ${SpellsRestrictedByZone.Arg[${a},,]}
         }
      /next a
   }
/return


Sub BuildRestrictedList
   /declare b int local
   /declare tString1 string local
   /declare tString2 string local
   /declare tString3 string local
   /declare tRestricted string local
   /declare tSpellID int local
   /varset tString1 ${Ini["${MBCommon}",RestrictedList,"${Zone.ShortName}","NOTFOUND"]}
   /if (!${tString1.Length} || ${tString1.Equal[NOTFOUND]}) {
|Look for the old format (using ZoneID)
      /varset tString1 ${Ini["${MBCommon}",RestrictedList,"${Zone.ID}","NOTFOUND"]}
      /if (${tString1.Length} && ${tString1.NotEqual[NOTFOUND]}) {
         /varset tString2 ${Zone.Name}
|Convert the old (SpellID|) format to (SpellName|SpellID)
         /for b 1 to ${tString1.Count[${PipeChar}]}
            /if (${Spell[${tString1.Arg[${b},${PipeChar}]}].ID}) {
               /varset tString2 ${tString2},${Spell[${tString1.Arg[${b},${PipeChar}]}].Name}${PipeChar}${Spell[${tString1.Arg[${b},${PipeChar}]}].ID}
            }
         /next b
         /varset tString1 ${tString2}
         /ini "${MBCommon}" "RestrictedList" "${Zone.ShortName}" "${tString1}"
      }
   }
   /if (${tString1.Find[,]}) {
      /varset tString3 ${tString1.Arg[1,,]}
      /for b 2 to ${Math.Calc[${tString1.Count[,]}+1]}
         /varset tString2 ${tString1.Arg[${b},,].Arg[2,${PipeChar}]}
         /if (${tString2.Length} && ${tString2.NotEqual[NULL]}) {
            /varset tSpellID ${tString2}
         } else {
            /varset tSpellID ${Spell[${tString1.Arg[${b},,].Arg[1,${PipeChar}]}].ID}
         }
         /if (${Spell[${tSpellID}].ID}) {
            /varset tString3 ${tString3},${Spell[${tSpellID}].Name}${PipeChar}${Spell[${tSpellID}].ID}
            /varset tRestricted ${If[!${tRestricted.Length},,${tRestricted},]}${Spell[${tSpellID}].ID}
         }
      /next b
      /if (${tString3.NotEqual[${tString1}]}) {
         /ini "${MBCommon}" "RestrictedList" "${Zone.ShortName}" "${tString3}"
      }
   }
   /varset SpellsRestrictedByZone ${tRestricted}
/return


Sub LoadSpellImmunes
   /declare a int local
   /declare tString string local
   /declare cCount int local
   /varset ZoneSpellImmunes
   /varset tString ${Ini["${MBCommon}","ImmuneList","${Zone.ShortName}","NOTFOUND"]}
   /if (${tString.Find[,]}) {
      /varset ZoneSpellImmunes ${tString.Right[-${tString.Find[,]}]}
   }
   /if (${ADCount}) {
      /for a 1 to ${ADCount}
         /varset ADImmune[${a}]
         /if (${Select[${ADSpellID[${a}]},${ZoneSpellImmunes}]}) {
            /varset tString ${Ini["${MBCommon}","ImmuneList","${Zone.ShortName}${PipeChar}${ADSpellID[${a}]}","NOTFOUND"]}
            /if (${tString.Arg[3,,].NotEqual[NULL]}) {
               /varset cCount ${Math.Calc[${tString.Count[,]}+1]}
               /varset ADImmune[${a}] ${tString.Arg[${cCount},,]}
            }
         }
      /next a
   }
/return


Sub AddToImmuneList(int tSpellID,string tName)
   /declare a int local
   /declare tString1 string local
   /declare tString2 string local
   /if (!${Spell[${tSpellID}].ID} || !${tName.Length}) /return FALSE
   /bc Msg: Adding ${tName} to the immune list.
   /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Adding ${tName} to the immune list.
| First Zone Entry (ZoneShortName=ZoneName,SpellID1,SpellID2,...
   /varset tString1 ${Ini["${MBCommon}","ImmuneList","${Zone.ShortName}","NOTFOUND"]}
   /if (${tString1.Find[,]}) {
      /varset tString2 ${tString1.Right[-${tString1.Find[,]}]}
      /if (!${Select[${tSpellID},${tString2}]}) {
         /varset tString2 ${tString1},${tSpellID}
      } else {
         /varset tString2 ${tString1}
      }
   } else {
      /varset tString2 ${Zone.Name},${tSpellID}
   }
   /if (${tString2.NotEqual[${tString1}]}) {
      /ini "${MBCommon}" "ImmuneList" "${Zone.ShortName}" "${tString2}"
   }
   /varset ZoneSpellImmunes ${tString2.Right[-${tString2.Find[,]}]}
| Secondary Zone Entries (ZoneShortName|SpellID=ZoneName,SpellName,|NPCName1||NPCName2|...
   /varset tString1 ${Ini["${MBCommon}","ImmuneList","${Zone.ShortName}${PipeChar}${tSpellID}","NOTFOUND"]}
   /if (${tString1.Arg[3,,].NotEqual[NULL]}) {
      /varset tString2 ${tString1.Arg[3,,]}
      /if (!${tString2.Find[${PipeChar}${tName}${PipeChar}]} && !${tString2.Find[${PipeChar}${tName}*${PipeChar}]}) {
         /varset tString2 ${tString1}${PipeChar}${tName}${PipeChar}
      } else {
         /varset tString2 ${tString1}
      }
   } else {
      /varset tString2 ${Zone.Name},${Spell[${tSpellID}].Name},${PipeChar}${tName}${PipeChar}
   }
   /if (${tString2.NotEqual[${tString1}]}) {
      /ini "${MBCommon}" "ImmuneList" "${Zone.ShortName}${PipeChar}${tSpellID}" "${tString2}"
   }
   /if (${ADCount}) {
      /for a 1 to ${ADCount}
         /if (${ADSpellID[${a}]}==${tSpellID}) {
            /varset ADImmune[${a}] ${tString2.Arg[3,,]}
         }
      /next a
   }
/return TRUE   

   
Sub Event_FeignD
   /echo Event FeignD!!!
   /if (${Me.PctHPs} > 20) {
      /if (${Me.State.Equal[STAND]}) /sit
      /delay 1s ${Me.State.Equal[SIT]}
      /if (${Me.State.Equal[SIT]}) /stand
      /delay 1s ${Me.Standing}
      /if (!${Me.Standing}) /stand
   }
/return


Sub Event_OoR
   /autoinv
/return


| Add item to comma-separated list
Sub AddToList(string List,string Item,string Options)
   /if (!${Item.Length}) /return ${List}
   /if (!${Select[${Item},${List}]} || ${Options.Find[DupOK]}) {
      /varset List ${List}${If[!${List.Length},,,]}${Item}
   }
/return ${List}


| Remove item from comma-separated list
Sub RemoveFromList(string List,string Item,string Options)
   /if (!${List.Length} || !${Item.Length}) /return ${List}
   /declare i int local ${Select[${Item},${List}]}
   /declare j int local
   /declare NewList string local
   /if (!${i}) /return ${List}   
   /for j 1 to ${Math.Calc[${List.Count[,]}+1]}
      /if (${j}!=${i}) {
         /varset NewList ${NewList}${If[!${NewList.Length},,,]}${List.Arg[${j},,]}
      }
   /next j
/return ${NewList}


Sub Event_Skillup(line,skillname,skillup)
   /if (${EchoSkillups}) {
      /echo ${If[${TimeStampOnEcho},[${Time}] ,]}Your ${skillname} increased to ${skillup}
   }
/return

