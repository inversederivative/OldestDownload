# Oldest File in Downloads

---

### Introduction

Similar to my Temperature Converter Project, I saw 
a Facebook post, which just said, "What's the 
oldest file in your downloads folder?"

I thought, why not do this in C++? It's gotta be
doable, right?

---

### Accessing the Downloads Folder

For both Windows and Linux, we have an environment 
variable we can use to get to the current user's 
"home" directory.

For Linux, we have:

    $home

And for Windows, we have:

    %USERPROFILE%

From here it should be pretty straightforward.

---


