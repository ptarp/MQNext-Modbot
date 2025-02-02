
Sub alerts_Setup
/RETURN

SUB alerts_Background_Events
 |/doevents
  /doevents charFlag

  /if (${Zone.ShortName.Equal[potactics]}) {
    /doevents tactics_stampede
  } else {
    /doevents tactics_stampede flush
  }

  /if (${Zone.ShortName.Equal[ikkinz]}) {
    /doevents setClass_Warrior
    /doevents setClass_Shaman
    /doevents setClass_Beastlord
    /doevents setClass_Necromancer
    /doevents setClass_Cleric
    /doevents setClass_Shadowknight
    /doevents setClass_Monk
    /doevents setClass_Bard
    /doevents setClass_Paladin
    /doevents setClass_Rogue
    /doevents setClass_Enchanter
    /doevents setClass_Wizard
    /doevents setClass_Berserker
    /doevents setClass_Magician
    /doevents setClass_Druid
    /doevents setClass_Ranger
    /doevents startClass_Priests
    /doevents startClass_Hybrids
    /doevents startClass_Casters
    /doevents startClass_Melees
    /doevents MobSet
  } else {
    /doevents setClass_Warrior flush
    /doevents setClass_Shaman flush
    /doevents setClass_Beastlord flush
    /doevents setClass_Necromancer flush
    /doevents setClass_Cleric flush
    /doevents setClass_Shadowknight flush
    /doevents setClass_Monk flush
    /doevents setClass_Bard flush
    /doevents setClass_Paladin flush
    /doevents setClass_Rogue flush
    /doevents setClass_Enchanter flush
    /doevents setClass_Wizard flush
    /doevents setClass_Berserker flush
    /doevents setClass_Magician flush
    /doevents setClass_Druid flush
    /doevents setClass_Ranger flush
    /doevents startClass_Priests flush
    /doevents startClass_Hybrids flush
    /doevents startClass_Casters flush
    /doevents startClass_Melees flush
  }
  /if (${Zone.ShortName.Equal[inktuta]}) {
    /doevents InktutaDeathtouch
  } else {
    /doevents InktutaDeathtouch flush
  }
  /if (${Zone.ShortName.Equal[uqua]}) {
    /doevents Define_UquaKey
  } else {
    /doevents Define_UquaKey flush
  }

  /if (${Zone.ShortName.Equal[chambersc]}) {
    /doevents goDuck
    /doevents dragornPBAE
    /doevents dragornBlast
    /doevents dragornThorn
    /doevents dragornReflect
  } else {
    /doevents goDuck flush
    /doevents dragornPBAE flush
    /doevents dragornBlast flush
    /doevents dragornThorn flush
    /doevents dragornReflect flush
  }

  /if (${Zone.ShortName.Equal[anguish]}) {
    /doevents angmask
  } else {
    /doevents angmask flush
  }
/RETURN

#Event angmask "#*#You feel a gaze of deadly power focusing on you#*#"
Sub Event_angmask
  /rs ~~~Mask on Me~~~ Ready: ${Me.Inventory[face].Name.Equal[Mirrored Mask]}
  /if (!${FindItem[=Mirrored Mask]}) {
    /bc [+r+] I dont have a Mirrored Mask
    /return
  } else /if (${FindItem[=Mirrored Mask].ItemSlot} >=23 ) {
    /if (${Me.Casting.ID}) /call interrupt
    /delay 3s !${Me.Casting.ID}
    /declare OMM_Mask string local ${Me.Inventory[face].Name}
    |/call WriteToIni "${MacroData_Ini},${Me.CleanName}-${MacroQuest.Server},Pending Exchange" "${OMM_Mask}/face" 1
    /delay 3
    /echo calling swapitem
    /exchange "Mirrored Mask" "face"
    /delay 5 ${Me.Inventory[face].Name.Equal[Mirrored Mask]}
    |/call SwapItem "Mirrored Mask" "face"
    |/delay 5 ${Me.Inventory[face].Name.Equal[Mirrored Mask]}
    |/call SwapItem "Mirrored Mask" "face"
    |/delay 5 ${Me.Inventory[face].Name.Equal[Mirrored Mask]}
  }
  /if (${Me.Inventory[face].Name.Equal[Mirrored Mask]}) {
    /declare numtries int local=0
    /if (${Me.Casting.ID}) /call interrupt
    /delay 3s !${Me.Casting.ID}
    :retry
    /varcalc numtries ${numtries}+1
    /casting "Mirrored Mask" -maxtries|3
    /delay 1s
    /if (!${Bool[${Me.Song[Reflective Skin]}]} && ${numtries} < 8) /goto :retry
  }
  |/if (${OMM_Mask.Length}) /call SwapItem "${OMM_Mask}" "face"
  /rs ~~~Mask on Me~~~ Ready:${Me.Inventory[face].Name.Equal[Mirrored Mask]}
/return


#Event tactics_stampede "#*#You hear the pounding of hooves#*#"
Sub Event_tactics_stampede
	/if (${Zone.ShortName.Equal[potactics]}) {
		/gsay STAMPEDE!
	}
/return

#Event charFlag "#*#You receive a character flag#*#"
Sub Event_charFlag
	/bc I have recieved a character flag!
/return

#Event Define_UquaKey "#*#The #1# must unlock the door to the next room.#*#"
SUB Event_Define_UquaKey(string line, string KeyToUse)
	/rs >>^<< The ${KeyToUse} unlocks the door >>^<<
	/popup >>^<< The ${KeyToUse} unlocks the door >>^<<
/return

|#Event setClass_Warrior "#*#Brute force and brawn#*#"
|#Event setClass_Shaman "#*#Cringes at the appearance of talismans#*#"
|#Event setClass_Beastlord "#*#Deep gashes of feral savagery#*#"
|#Event setClass_Necromancer "#*#Doom of death#*#"
|#Event setClass_Cleric "#*#Dread of celestial spirit#*#"
|#Event setClass_Shadowknight "#*#It appears that this creature dreads the strike of death#*#"
|#Event setClass_Monk "#*#Focused tranquility#*#"
|#Event setClass_Bard "#*#Foreboding melody#*#"
|#Event setClass_Paladin "#*#fears a holy blade#*#"
|#Event setClass_Rogue "#*#Ignores anything behind it#*#"
|#Event setClass_Enchanter "#*#Mind and body are vulnerable#*#"
|#Event setClass_Wizard "#*#Falters when struck with the power of the elements#*#"
|#Event setClass_Berserker "#*#Shies from heavy blades#*#"
|#Event setClass_Magician "#*#Summoned elements#*#"
|#Event setClass_Druid "#*#The creature seems weak in the face of the power of nature#*#"
|#Event setClass_Ranger "#*#True shots and fast blades#*#"

SUB Event_setClass_Warrior
  /popup Spawn must be killed by a >>^<< WARRIOR >>^<<
  /if (${Raid.Leader.Name.Equal[${Me}]}) /rs Spawn must be killed by a >>^<< WARRIOR >>^<<
  /if (${Select[${Me.Class.ShortName},war]}) {
      /target id ${ACMATarget}
        /varset DoMelee True
        /varset DoDebuffs TRUE
        /killthis
  } else {
      /call WrongClass
  }
/return

SUB Event_setClass_Shaman
	/popup Spawn must be killed by a >>^<< SHAMAN >>^<<
  /if (${Raid.Leader.Name.Equal[${Me}]}) /rs Spawn must be killed by a >>^<< SHAMAN >>^<<
    /if (${Select[${Me.Class.ShortName},shm]}) {
      /target id ${ACMATarget}
        /varset DoMelee True
        /varset DoDebuffs TRUE
        /varset DoHeals FALSE
        /killthis
  } else {
      /call WrongClass
  }
/return

SUB Event_setClass_Beastlord
	/popup Spawn must be killed by a >>^<< BEASTLORD >>^<<
  /if (${Raid.Leader.Name.Equal[${Me}]}) /rs Spawn must be killed by a >>^<< BEASTLORD >>^<<
    /if (${Select[${Me.Class.ShortName},bst]}) {
      /target id ${ACMATarget}
        /varset DoMelee True
        /varset DoDebuffs TRUE
        /varset DoHeals FALSE
        /killthis
  } else {
      /call WrongClass
  }
/return

SUB Event_setClass_Necromancer
	/popup Spawn must be killed by a >>^<< NECROMANCER >>^<<
  /if (${Raid.Leader.Name.Equal[${Me}]}) /rs Spawn must be killed by a >>^<< NECROMANCER >>^<<
    /if (${Select[${Me.Class.ShortName},nec]}) {
      /target id ${ACMATarget}
        /varset DoMelee True
        /varset DoDebuffs TRUE
        /killthis
  } else {
      /call WrongClass
  }
/return

SUB Event_setClass_Cleric
	/popup Spawn must be killed by a >>^<< CLERIC >>^<<
  /if (${Raid.Leader.Name.Equal[${Me}]}) /rs Spawn must be killed by a >>^<< CLERIC >>^<<
    /if (${Select[${Me.Class.ShortName},clr]}) {
      /target id ${ACMATarget}
        /varset DoMelee True
        /varset DoDebuffs TRUE
        /varset DoHeals FALSE
        /killthis
  } else {
      /call WrongClass
  }
/return

SUB Event_setClass_Shadowknight
	/popup Spawn must be killed by a >>^<< SHADOWKNIGHT >>^<<
  /if (${Raid.Leader.Name.Equal[${Me}]}) /rs Spawn must be killed by a >>^<< SHADOWKNIGHT >>^<<
    /if (${Select[${Me.Class.ShortName},shd]}) {
      /target id ${ACMATarget}
        /varset DoMelee True
        /varset DoDebuffs TRUE
        /killthis
  } else {
      /call WrongClass
  }
/return

SUB Event_setClass_Monk
	/popup Spawn must be killed by a >>^<< MONK >>^<<
  /if (${Raid.Leader.Name.Equal[${Me}]}) /rs Spawn must be killed by a >>^<< MONK >>^<<
    /if (${Select[${Me.Class.ShortName},mnk]}) {
      /target id ${ACMATarget}
        /varset DoMelee True
        /varset DoDebuffs TRUE
        /killthis
  } else {
      /call WrongClass
  }
/return

SUB Event_setClass_Bard
	/popup Spawn must be killed by a >>^<< BARD >>^<<
  /if (${Raid.Leader.Name.Equal[${Me}]}) /rs Spawn must be killed by a >>^<< BARD >>^<<
    /if (${Select[${Me.Class.ShortName},brd]}) {
      /target id ${ACMATarget}
        /varset DoMelee True
        /varset DoDebuffs TRUE
        /killthis
  } else {
      /call WrongClass
  }
/return

SUB Event_setClass_Paladin
  /popup Spawn must be killed by a >>^<< PALADIN >>^<<
  /if (${Raid.Leader.Name.Equal[${Me}]}) /rs Spawn must be killed by a >>^<< PALADIN >>^<<
    /if (${Select[${Me.Class.ShortName},pal]}) {
      /target id ${ACMATarget}
        /varset DoMelee True
        /varset DoDebuffs TRUE
        /killthis
  } else {
      /call WrongClass
  }
/return

SUB Event_setClass_Rogue
	/popup Spawn must be killed by a >>^<< ROGUE >>^<<
  /if (${Raid.Leader.Name.Equal[${Me}]}) /rs Spawn must be killed by a >>^<< ROGUE >>^<<
    /if (${Select[${Me.Class.ShortName},rog]}) {
      /target id ${ACMATarget}
        /varset DoMelee True
        /varset DoDebuffs TRUE
        /killthis
  } else {
      /call WrongClass
  }
/return

SUB Event_setClass_Enchanter
	/popup Spawn must be killed by a >>^<< ENCHANTER >>^<<
  /if (${Raid.Leader.Name.Equal[${Me}]}) /rs Spawn must be killed by a >>^<< ENCHANTER >>^<<
    /if (${Select[${Me.Class.ShortName},enc]}) {
      /target id ${ACMATarget}
        /varset DoMelee True
        /varset DoDebuffs TRUE
        /killthis
  } else {
      /call WrongClass
  }
/return

SUB Event_setClass_Wizard
	/popup Spawn must be killed by a >>^<< WIZARD >>^<<
  /if (${Raid.Leader.Name.Equal[${Me}]}) /rs Spawn must be killed by a >>^<< WIZARD >>^<<
    /if (${Select[${Me.Class.ShortName},wiz]}) {
      /target id ${ACMATarget}
        /varset DoMelee True
        /varset DoDebuffs TRUE
        /killthis
  } else {
      /call WrongClass
  }
/return

SUB Event_setClass_Berserker
	/popup Spawn must be killed by a >>^<< BERSERKER >>^<<
  /if (${Raid.Leader.Name.Equal[${Me}]}) /rs Spawn must be killed by a >>^<< BERSERKER >>^<<
    /if (${Select[${Me.Class.ShortName},ber]}) {
      /target id ${ACMATarget}
        /varset DoMelee True
        /varset DoDebuffs TRUE
        /killthis
  } else {
      /call WrongClass
  }
/return

SUB Event_setClass_Magician
	/popup Spawn must be killed by a >>^<< MAGICIAN >>^<<
  /if (${Raid.Leader.Name.Equal[${Me}]}) /rs Spawn must be killed by a >>^<< MAGICIAN >>^<<
    /if (${Select[${Me.Class.ShortName},mag]}) {
      /target id ${ACMATarget}
        /varset DoMelee True
        /varset DoDebuffs TRUE
        /killthis
  } else {
      /call WrongClass
  }
/return

SUB Event_setClass_Druid
	/popup Spawn must be killed by a >>^<< DRUID >>^<<
  /if (${Raid.Leader.Name.Equal[${Me}]}) /rs Spawn must be killed by a >>^<< DRUID >>^<<
    /if (${Select[${Me.Class.ShortName},dru]}) {
      /target id ${ACMATarget}
        /varset DoMelee True
        /varset DoDebuffs TRUE
        /varset DoHeals FALSE
        /killthis
  } else {
      /call WrongClass
  }
/return
SUB Event_setClass_Ranger
	/popup Spawn must be killed by a >>^<< RANGER >>^<<
  /if (${Raid.Leader.Name.Equal[${Me}]}) /rs Spawn must be killed by a >>^<< RANGER >>^<<
    /if (${Select[${Me.Class.ShortName},rng]}) {
      /target id ${ACMATarget}
        /varset DoMelee True
        /varset DoDebuffs TRUE
        /varset DoHeals FALSE
        /killthis
  } else {
      /call WrongClass
  }
/return

|#Event startClass_Priests "#*#The creature cannot stand up to the power of healers#*#"
|#Event startClass_Hybrids "#*#The creature appears weak to the combined effort of might and magic#*#"
|#Event startClass_Casters "#*#The creature will perish under the strength of intelligent magic#*#"
|#Event startClass_Melees "#*#The creature appears weak to the combined effort of strength and cunning#*#"
SUB Event_startClass_Priests
  /if (${Raid.Leader.Name.Equal[${Me}]}) /rs Spawn must be killed by a >>^<< PRIEST >>^<<
  /if (${Select[${Me.Class.ShortName},shm,clr,dru]}) {
      /multiline ; /target id ${Spawn[Vrex Xalkax`s Marauder].ID};/varset ACMATarget ${Spawn[Vrex Xalkax`s Marauder].ID}
        /varset DoMelee True
        /varset DoDebuffs TRUE
        /if (${Select[${Me.Class.ShortName},dru]}) /varset DoHeals FALSE
        /killthis
  } else {
      /call WrongClass
  }
/return

SUB Event_startClass_Hybrids
  /if (${Raid.Leader.Name.Equal[${Me}]}) /rs Spawn must be killed by a >>^<< HYBRID >>^<<
  /if (${Select[${Me.Class.ShortName},rng,bst,brd,shd,pal]}) {
      /multiline ; /target id ${Spawn[Vrex Xalkax`s Marauder].ID};/varset ACMATarget ${Spawn[Vrex Xalkax`s Marauder].ID}
        /varset DoMelee True
        /varset DoDebuffs TRUE
        /varset DoHeals FALSE
        /killthis
  } else {
      /call WrongClass
  }
/return

SUB Event_startClass_Casters
  /if (${Raid.Leader.Name.Equal[${Me}]}) /rs Spawn must be killed by a >>^<< CASTER >>^<<
  /if (${Select[${Me.Class.ShortName},mag,wiz,enc,nec]}) {
      /multiline ; /target id ${Spawn[Vrex Xalkax`s Marauder].ID};/varset ACMATarget ${Spawn[Vrex Xalkax`s Marauder].ID}
        /varset DoMelee True
        /varset DoDebuffs TRUE
        /killthis
  } else {
      /call WrongClass
  }
/return

SUB Event_startClass_Melees
  /if (${Raid.Leader.Name.Equal[${Me}]}) /rs Spawn must be killed by a >>^<< MELEE >>^<<
  /if (${Select[${Me.Class.ShortName},war,rog,mnk,ber]}) {
      /multiline ; /target id ${Spawn[Vrex Xalkax`s Marauder].ID};/varset ACMATarget ${Spawn[Vrex Xalkax`s Marauder].ID}
        /varset DoMelee True
        /varset DoDebuffs TRUE
        /killthis
  } else {
      /call WrongClass
  }
/return

#Event InktutaDeathtouch "#*#thoughts of a cursed trusik invade your mind#*#"
SUB Event_InktutaDeathtouch
  /rs I, >>^<< ${Me.Name} >>^<<, who am about to die, salute you!!
/return

#Event goDuck "#*#From the corner of your eye, you notice a Kyv taking aim at your head. You should duck."
#Event dragornPBAE "#*#You notice that the Dragorn before you is preparing to cast a devastating close-range spell."
#Event dragornBlast "You notice that the Dragorn before you is making preparations to cast a devastating spell.  Doing enough damage to him might interrupt the process."
#Event dragornThorn "#*#The Dragorn before you is sprouting sharp spikes."
#Event dragornReflect "#*#The Dragorn before you is developing an anti-magic aura."

Sub Event_dragornPBAE
  /popup Dragorn PBAE Inc
  /if (${Raid.Leader.Name.Equal[${Me}]}) /rs @@@ PBAE INC @@@
/return

Sub Event_dragornBlast
  /popup Dragorn Blast Inc
  /if (${Raid.Leader.Name.Equal[${Me}]}) /rs @@@ BLAST INC @@@
/return

Sub Event_dragornThorn
  /popup Dragorn Thorns Inc
  /if (${Raid.Leader.Name.Equal[${Me}]}) /rs ^^^ Thorns ON ^^^
/return

Sub Event_dragornReflect
  /popup Dragorn Reflect Inc
  /if (${Raid.Leader.Name.Equal[${Me}]}) /rs ~~~ Reflect ON ~~~
/return

Sub Event_goDuck
  /echo in goduck
  |/call AssistOff
  /squelch /target clear
  /declare duckchecks int local 0
  /declare pcthp int local ${Me.PctHPs}
  :checkDuck
  /varcalc duckchecks ${duckchecks}+1
  /if (!${Me.Ducking}) /keypress duck
  /delay 3
  /if (${duckchecks} < 11 ) /goto :checkDuck

  /if (${Me.Ducking}) /keypress duck
  /if (${Me.PctHPs} < ${pcthp}) /echo looks like i failed Event_goDuck: origHP ${pcthp} currHP ${Me.PctHPs}
/return

SUB alerts_CharacterSettings
/return
Sub alerts_Aliases
/return
Sub alerts_MacroSettings
/return

Sub WrongClass
    /declare tTauntTarget int local ${If[${Target.Type.Equal[npc]},${Target.ID},0]}
    /declare tTaunting bool local ${Melee.AggroMode}
    /if (${OffTank} && ${ACMATarget} && ${NetBots[${TankName}].TargetID} && ${Target.ID}!=${NetBots[${TankName}].TargetID}) /return
    /varset ACMATarget 0
    /squelch /target clear
    /if (${Me.Combat}) /attack off
    /if (${Me.AutoFire}) /autofire off
     /if (${ACState}) /varcalc ACState 0
     /if (${Me.Casting.ID} && !${AHHealList.Find[${Me.Casting.Name}]}) /multiline ; /squelch /stoptwist;/interrupt;/squelch /stoptwist
     |/if (${Me.Mount.ID}) /dismount
     /multiline ; /varset DoMelee FALSE;/varset DoDebuffs FALSE;/varset DoRanged FALSE;/bc Setting - DoDebuffs, DoMelee and DoRanged FALSE
     /if (${Me.Class.ShortName.Equal[brd]}) /twist ${BardRestGems}
     /if (${tTaunting}) {
         /target id ${tTauntTarget}
         /delay 1s ${Target.ID}==${tTauntTarget}
         /if (${Me.AbilityReady[Taunt]}) /doability taunt
         /if (${Me.CombatAbility[Bazu Bellow]}) {
             /disc Bazu
         } else /if (${Me.CombatAbility[Incite]}) {
             /disc Incite
         }
     }
/return