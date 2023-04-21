# Project3

Download item_exported.csv from 
https://drive.google.com/file/d/14PY7x5N6pnH83fFYoKDSyaoBtiiITew_/view?usp=share_link

and place in C++ folder where it needs to be to access files for iostream

search function returns pair<string, vector<string>>
first value in pair is the channel ID
second value in pair is vector of the channel's attributes in string format in the order of the csv.

channelPointer->second.at(i) will contain the returned pair from the search function if found (otherwise prints oh brother this guy stinks)
  
channelPointer->second.at(0) = video comment count
  
1 = channel comment count
  
2 = channel id
  
3 - channel view count
  
4 - channel elapsed time
  
5 - comments / subscriber
  
6 - comments / views
  
7 - dislikes / subscriber
  
8 - dislikes / views
  
9 - elapsedtime
  
10 - likes/dislikes
  
11 - likes/subscriber
  
12 - likes/views
  
13 - subscriber count
  
14 - totalviews / channel elapsed time
  
15 - totvideos / videocount
  
16 - totviews / totsubs
  
17 - videoCategoryID
  
18 - videoCount
  
19 - videoDislikeCount
  
20 - videoID
  
21 - videoLikeCount
  
22 - videoPublished
  
23 - videoViewCount
  
24 - views/elapsedtime
  
25 - views/subscribers
