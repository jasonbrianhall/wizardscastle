import random

def look_northeast(game):
	temp= game.get("character").get("x")-1
	temp2=game.get("character").get("y")-1
	castlesize=game.get("castle").get("size")
	if temp<1:
		temp=castlesize
	if temp2<1:
		temp2=castlesize
	game["character"]["moved"]=True
	X=str(temp)
	Y=str(temp2)
	Z=str(game.get("character").get("z"))
	game["castle"][X][Y][Z]["explored"]=True	
	return game

def look_northwest(game):
	temp= game.get("character").get("x")-1
	temp2=game.get("character").get("y")+1
	castlesize=game.get("castle").get("size")
	if temp<1:
		temp=castlesize
	if temp2>castlesize:
		temp2=1
	game["character"]["moved"]=True
	X=str(temp)
	Y=str(temp2)
	Z=str(game.get("character").get("z"))
	game["castle"][X][Y][Z]["explored"]=True	
	return game

def look_southeast(game):
	temp= game.get("character").get("x")+1
	temp2=game.get("character").get("y")-1
	castlesize=game.get("castle").get("size")
	if temp>castlesize:
		temp=1
	if temp2<1:
		temp2=castlesize
	game["character"]["moved"]=True
	X=str(temp)
	Y=str(temp2)
	Z=str(game.get("character").get("z"))
	game["castle"][X][Y][Z]["explored"]=True	
	return game

def look_southwest(game):
	temp= game.get("character").get("x")+1
	temp2=game.get("character").get("y")+1
	castlesize=game.get("castle").get("size")
	if temp>castlesize:
		temp=1
	if temp2>castlesize:
		temp2=1
	game["character"]["moved"]=True
	X=str(temp)
	Y=str(temp2)
	Z=str(game.get("character").get("z"))
	game["castle"][X][Y][Z]["explored"]=True	
	return game



def look_north(game):
	temp=game.get("character").get("x")-1
	castlesize=game.get("castle").get("size")
	if temp<1:
		temp=castlesize
	game["character"]["moved"]=True
	X=str(temp)
	Y=str(game.get("character").get("y"))
	Z=str(game.get("character").get("z"))
	game["castle"][X][Y][Z]["explored"]=True	
	return game

def look_south(game):
	temp=game.get("character").get("x")+1
	castlesize=game.get("castle").get("size")
	if temp>castlesize:
		temp=1
	game["character"]["moved"]=True
	X=str(temp)
	Y=str(game.get("character").get("y"))
	Z=str(game.get("character").get("z"))
	game["castle"][X][Y][Z]["explored"]=True	
	return game
	
def look_east(game):
	temp=game.get("character").get("y")+1
	castlesize=game.get("castle").get("size")
	if temp>castlesize:
		temp=1
	game["character"]["moved"]=True
	X=str(game.get("character").get("x"))
	Y=str(temp)
	Z=str(game.get("character").get("z"))
	game["castle"][X][Y][Z]["explored"]=True	
	return game

def look_west(game):
	temp=game.get("character").get("y")-1
	castlesize=game.get("castle").get("size")
	if temp<1:
		temp=castlesize
	game["character"]["moved"]=True
	X=str(game.get("character").get("x"))
	Y=str(temp)
	Z=str(game.get("character").get("z"))
	game["castle"][X][Y][Z]["explored"]=True	
	return game


def go_north(game):
	game["character"]["x"]=game.get("character").get("x")-1
	castlesize=game.get("castle").get("size")
	if game.get("character").get("x")<1:
		game["character"]["x"]=castlesize
	game["character"]["moved"]=True
	X=str(game.get("character").get("x"))
	Y=str(game.get("character").get("y"))
	Z=str(game.get("character").get("z"))
	game["castle"][X][Y][Z]["explored"]=True	
	return game

def go_south(game):
	game["character"]["x"]=game.get("character").get("x")+1
	castlesize=game.get("castle").get("size")
	if game.get("character").get("x")>castlesize:
		game["character"]["x"]=1
	game["character"]["moved"]=True
	X=str(game.get("character").get("x"))
	Y=str(game.get("character").get("y"))
	Z=str(game.get("character").get("z"))
	game["castle"][X][Y][Z]["explored"]=True	

	return game

def go_east(game):
	game["character"]["y"]=game.get("character").get("y")+1
	castlesize=game.get("castle").get("size")
	if game.get("character").get("y")>castlesize:
		game["character"]["y"]=1
	game["character"]["moved"]=True
	X=str(game.get("character").get("x"))
	Y=str(game.get("character").get("y"))
	Z=str(game.get("character").get("z"))
	game["castle"][X][Y][Z]["explored"]=True	

	return game

def go_west(game):
	game["character"]["y"]=game.get("character").get("y")-1
	castlesize=game.get("castle").get("size")
	if game.get("character").get("y")<1:
		game["character"]["y"]=castlesize
	game["character"]["moved"]=True
	X=str(game.get("character").get("x"))
	Y=str(game.get("character").get("y"))
	Z=str(game.get("character").get("z"))
	game["castle"][X][Y][Z]["explored"]=True	

	return game

def go_sinkhole(game):

	print("You found a sinkhole!!\n")

	game["character"]["z"]=game.get("character").get("z")-1
	castlesize=game.get("castle").get("size")
	if game.get("character").get("z")<1:
		game["character"]["z"]=castlesize
	game["character"]["moved"]=True
	X=str(game.get("character").get("x"))
	Y=str(game.get("character").get("y"))
	Z=str(game.get("character").get("z"))
	game["castle"][X][Y][Z]["explored"]=True	

	return game


def go_up(game):
	return game

def go_down(game):
	return game
	
def go_drink(game):
	return game
	
def go_map(game):

	print("\n")
	castlesize=game.get("castle").get("size")
	multiplier=(castlesize*13)+1
	level=str(game.get("character").get("z"))
	for x in range(1,castlesize+1):
		X=str(x)
		if x==1:
			print("*"*multiplier)
		else:
			print("|\n" + "*"*multiplier)
		for y in range(1,castlesize+1):
			print("|", end="")
			Y=str(y)
			
			tempx=str(game.get("character").get("x"))
			tempy=str(game.get("character").get("y"))
			if tempx==X and tempy==Y:
				print("You are here", end="")
			else:
				if game.get("castle").get(X).get(Y).get(level).get("explored")==True:
					if game.get("castle").get(X).get(Y).get(level).get("contents")=={}:
						print(" "*12, end="")
					else:
						for contents in game.get("castle").get(X).get(Y).get(level).get("contents"):
							length=len(contents)
							filler=11-length
							print(" " + contents, end="")
							print(" "*filler, end="")
							
							break
				else:
					print(" " + "?"*10+" ", end='')
				
	print("|\n" + "*"*multiplier + "\n")
				
				
				
	
	return game

def go_flare(game):
	game=look_north(game)
	game=look_south(game)
	game=look_east(game)
	game=look_west(game)
	game=look_northeast(game)
	game=look_southeast(game)
	game=look_northwest(game)
	game=look_southwest(game)

	return game

def go_lamp(game):
	return game
	
def go_open(game):
	return game

def go_gaze(game):
	return game

def go_teleport(game):
	return game

def ambience():
	print("\nYou ", end="")

	data={
		"0": {
			"init": "stepped on a",
			"random": [
				" frog!",
				" toad!",
				" fairie!",
				" brownie!",
				" a chicken egg!"
			],
			"post": [
				" You wipe it off your boots.",
				"",
				"",
				" You are reminded to buy new boots if you live."
				]
		},
		"1": {
			"init": "hear",
			"random": [
				" a scream!",
				" footsteps!",
				" a wumpus!",
				" thunder!",
				" a scream!  You think someone is being tortured!",
				" a rat!"
				],
			"post": [
				" Your heart starts racing.",
				"",
				"",
				"",
				"",
				" You want to go home."
				]
			},
		"2": {
			"init": "sneezed!",
			"random": [
				" You think a monster heard you!",
				" Now you have to pee!",
				"",
				"",
				""
				],
			"post": [
				" You think about trying to make less noise.",
				"",
				"",
				"",
				"",
				" Now you farted."
				]
			},
		"3": {
			"init": "see a bat fly by!",
			"random": [
				" Pray it's not a vampire bat.",
				""
				],
			"post": [
				" It flies a little too close for comfort.",
				"",
				"",
				""
				]
			},
		"4": {
			"init": "smell",
			"random": [
				" a kobold",
				" an orc",
				" a wolf",
				" a goblin",
				" an ogre",
				" a troll",
				" a bear",
				" a minotaur",
				" a gargoyle",
				" a chimera",
				" a dragon",
				],
			"post": [
				" frying!!!",
				" roasting!!!",
				" cooking!!!",
				"\'s entrails!",
				"\'s stool!",
				"\'s breathe nearby!"
				]
			},
		"5": {
			"init": "feel like you are being watched!",
			"random": [
				" You see glowing eyes!",
				" You think you hear someone talking."
				],
			"post": [
				" You realize it's only in your head.",
				" You swallow your fear."
				"",
				"",
				"",
				""
				]
			},
		"6": {
			"init": "hear faint rustling noises!",
			"random": [
				" They are getting closer!",
				""
				],
			"post": [
				" Now they are getting further away.",
				"",
				"",
				""
				]
			},
		"7": {
			"init": "feel like you are being watched!",
			"random": [
				" You see glowing eyes!",
				" You think you hear someone talking.",
				"",
				"",
				"",
				"",
				""
				],
			"post": [
				" You think you are going insane",
				""
				""
				""
				]
			},
		}
	



	randata=random.randint(0,10)
	if randata>7:
		randata=4

	ranstring=str(randata)
	mylist=data.get(ranstring).get("random")
	postlist=data.get(ranstring).get("post")
	
	print(data.get(ranstring).get("init") + random.choice(mylist) + random.choice(postlist))

def go_warp(game):
	
	print("\nYou found a warp!!\n")
	castlesize=game.get("castle").get("size")
	X=str(random.randint(1,castlesize))
	Y=str(random.randint(1,castlesize))
	Z=str(random.randint(1,castlesize))
	
	print("You are warping to " + X +"/"+Y+"/"+Z)
	game["character"]["x"]=X
	game["character"]["y"]=Y
	game["character"]["z"]=Z
	game["castle"][str(X)][str(Y)][str(Z)]["explored"]=True		
	
	return game

def go_monster(game):
	print("\nYou found a monster!!\n")
	return game

def do_nothing(game):
	return game

def action_room(game):
	actiondict={
		"warp":  go_warp,
		"monster": go_monster,
		"sinkhole": go_sinkhole,
		"entrance": do_nothing
	}

	if game.get("character").get("moved")==True:
		X=str(game.get("character").get("x"))
		Y=str(game.get("character").get("y"))
		Z=str(game.get("character").get("z"))
		print("Your current position is: " + X +"/"+Y+"/"+Z)
		game["character"]["moved"]=False

		for contents in game.get("castle").get(X).get(Y).get(Z).get("contents"):
			print("contents is", contents)
			if not contents=={}:
				game=actiondict.get(contents)(game)
			break
				
		

		randint=random.randint(0,3)
		if randint==0:
			ambience()
			
	return game

def castle_help(game):

	print(""""*** WIZARD'S CASTLE COMMAND AND INFORMATION SUMMARY ***

The following commands are available :

H/elp	  N/orth	S/outh	  E/ast		W/est	  U/p
D/own	  DR/ink	M/ap	  F/lare	L/amp	  O/pen
G/aze	  T/eleport Q/uit

the contents of rooms are as follows :"

. = empty room		b = book			c = chest
d = stairs down		e = entrance/exit	f = flares
g = gold pieces		m = monster			o = crystal orb
p = magic pool		s = sinkhole		t = treasure
u = stairs up		v = vendor			w = warp/orb

The benefits of having treasures are :

Ruby red - avoid lethargy	  Pale pearl - avoid leech
Green gem - avoid forgetting  Opal eye - cures blindness
Blue flame - dissolves books  Norn stone - no benefit
Palantir - no benefit		  Silmaril - no benefit

Press return when ready to resume""")

	input()
	
	return game

