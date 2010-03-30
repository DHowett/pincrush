" Vim syntax file
" Language: Logos (Objective-C++)
" Maintainer: Dustin Howett
" Latest Revision: December 22, 2009

if exists("b:current_syntax")
	finish
endif

runtime! syntax/objc.vim

syn match logosHookInit '%hook'
syn match logosHookInit '%subclass'
syn match logosKeyword '%\(log\|orig\(inal\)\=\|c\(onstruc\)\=tor\|init\|group\|new\|class\)%\='
syn match logosKeyword '%end%\='
syn region logosHook start='%hook' end='%end' fold transparent
syn region logosGroup start='%group' end='%end' fold transparent

let b:currentsyntax = "logos"
hi def link logosHookInit PreProc
hi def link logosKeyword PreProc
