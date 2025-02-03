# Parse-CC-Base-C-Plus-Plus

This is a parser for my personal notetaking, but I've uploaded it in case anyone else wants to take a crack at it. It 
converts your story notes into a JSON format using C++. A sample story note organization may look like-

```
Season 1
* Ep1: One Last Time
   * Characters
      * Virto Hatage
      * Isuka Brewa
      * Kiris Arien-Aver
   * Locations
      * Fields of Zipangu
      * Kiris’s Old Town of Beaufort, Zipangu
      * A random Orderite prison camp, Zipangu
      * Isuka’s Library, Zipangu
   * Start Date
      * June 1st, 2024
   * Timeline
      * Two days of events have passed. The next chapter will start on June 4th, 2024. 
   * Songs
      * Intro Song
         * Synthematic by Derek Moody
      * Scene Specific
         * Kiris’s Flashback
            * Pithoprakta by Iannis Xenakis
         * Setting up Shop
            * Three am by Quinn
         * Waking up
            * Real Friends by Curtis Schwartz
         * Kiris comes across knights
            * String Quartet No. 8, Part 1 by Dmitri Shostakovich
         * Isuka blows up the lunch hall
            * A Real Life by Greek Fire
         * Someone talks to Kiris
            * The Passing of Time by Marc-Oliver Dupin
      * Outro Song
         * Wolf in Sheep’s Clothing by Set It Off
```

The way it organizes stuff is by specific indentation in a txt document, which is shown further below. Use the text base 
file provided as a guide.

# Level One

Level One indentation will have nothing but the text starting immediately. Used for naming seasons.

# Level Two

Level Two indentation will have an asterisk before the text starting. Used for naming episodes.

# Level Three

Level Three indentation will have three spaces before the asterisk and text. Used for marking characters, locations, 
start dates, timelines, and songs.

# Level Four

Level Four indentation will have six spaces before the asterisk and text. Used for listing the content of the level 
three stuff and categorizing songs.

# Level Five

Level Five indentation will have nine spaces before the asterisk and text. Used for specifying scenes where specific 
songs are used.

# Level Six

Level Six indentation will have twelve spaces before the asterisk and text. Used for specifying songs for specific 
scenes.

To run this program, first build the project in Visual Studio, then run the script ParseBase.cpp. This was 
written using Visual Studio 2022 and C++14. You have to copy the file's path (AKA Casual Roleplay Base.txt) into the program terminal.

Running the program will return a folder called "Output" with the parsed JSON file of all your content, a list of characters, a list of 
locations, and a list of songs. It will be saved to the same folder as your input base file. The program WILL crash if it can't find what is needed.

Have fun!
