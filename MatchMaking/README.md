# Match Making API 
The match making API is designed to act as a facilitator for finding a match based on various possible criteria. 

The System should have calls for 

* Find match
* Check if Found

Which adds the person to a "queue" to see if they can be matched on given criteria. 
The Match maker should have inputs for the amount of matches, and what "criteria" they are being matched on. 

This could be used on a dating site, or a multiplayer game- so there should also be the ability to see if they need to be matched by being put into a group/game together, or if they should just be matched by returning the id of the other person. 

Check if Found should be used as a follow-up if a match was not previously found when "Find Match" was called, but has since been found by another user making a call to "find match". For a dating site, there should be criteria to match even if they are not online at the time. 

Given this, it is important that we continually add methods that can be called for how a match could be made, so that eventally the match making API can also rely on the analytics to see if the match is appropriate. 
