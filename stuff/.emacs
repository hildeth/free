(add-to-list 'load-path "~/git/contrib/emacs")
(require 'git)
(require 'git-blame)

(setq chpl-home (getenv "CHPL_HOME"))
(if (string= chpl-home nil) nil
  (add-to-list 'load-path (concat chpl-home "/etc/emacs/23")))
(autoload 'chpl-mode "chpl-mode" "Chpl enhanced cc-mode" t)

(custom-set-variables
  ;; custom-set-variables was added by Custom.
  ;; If you edit it by hand, you could mess it up, so be careful.
  ;; Your init file should contain only one such instance.
  ;; If there is more than one, they won't work right.
 '(comint-scroll-show-maximum-output nil)
 '(compilation-error-regexp-alist (quote (bash gnu gcc-include lcc makepp perl gcov-file gcov-header gcov-nomark gcov-called-line gcov-never-called)))
 '(delete-selection-mode nil)
 '(ediff-split-window-function (quote split-window-horizontally))
 '(gud-gdb-command-name "gdb --annotate=3 --args")
 '(indent-tabs-mode nil)
 '(inhibit-default-init t)
 '(inhibit-startup-screen t)
 '(menu-bar-mode nil)
 '(scroll-bar-mode (quote right))
 '(show-paren-mode t)
 '(tab-width 4)
 '(tool-bar-mode nil)
 '(transient-mark-mode t)
 '(truncate-partial-width-windows nil))
(custom-set-faces
  ;; custom-set-faces was added by Custom.
  ;; If you edit it by hand, you could mess it up, so be careful.
  ;; Your init file should contain only one such instance.
  ;; If there is more than one, they won't work right.
 '(default ((t (:stipple nil :background "black" :foreground "white" :inverse-video nil :box nil :strike-through nil :overline nil :underline nil :slant normal :weight normal :height 80 :family "fixed"))))
 '(diff-added ((t (:foreground "green"))))
 '(diff-changed ((t (:foreground "DodgerBlue"))))
 '(diff-removed ((t (:foreground "red"))))
 '(font-lock-comment-delimiter-face ((t (:foreground "green"))))
 '(font-lock-comment-face ((t (:foreground "green"))))
 '(font-lock-keyword-face ((t (:foreground "darkCyan"))))
 '(font-lock-type-face ((t (:foreground "cyan"))))
 '(font-lock-variable-name-face ((t (:foreground "purple"))))
 '(font-lock-warning-face ((((class color) (min-colors 88) (background dark)) (:foreground "Red" :weight bold)))))

(defconst my-c-style
 '((c-basic-offset . 2)
   ;(c-hanging-braces-alist . ((substatement-open after) (brace-list-open)))
   (c-offsets-alist
    (substatement-open . 0)
    (label . *)
    (case-label . *)
    (statement-case-intro . *)
   )
  )
 "hilde's C source code style"
)

(defconst my-c++-style
 '((c-basic-offset . 2)
   ;(c-hanging-braces-alist . ((substatement-open after) (brace-list-open)))
   (c-offsets-alist
    (substatement-open . 0)
    (label . *)
    (case-label . *)
    (access-label . /)
    (statement-case-intro . *)
   )
  )
 "hilde's C++ source code style"
)

(c-add-style "hildecc" my-c++-style)
(c-add-style "hilde" my-c-style)

(add-to-list 'auto-mode-alist '("\\.chpl$" . chpl-mode))
(add-to-list 'auto-mode-alist '("\\.future$" . text-mode))

(defun my-common-mode-hook ()
 (setq tab-width 4)
 (setq fill-column 79)
 (auto-fill-mode 1)
)

(defun my-text-mode-hook ()
 (my-common-mode-hook)
 (setq fill-column 90)
)

(defun my-c-mode-hook ()
 (my-common-mode-hook)
 (c-set-style "hilde")
)

(defun my-c++-mode-hook ()
 (my-common-mode-hook)
 (c-set-style "hildecc")
)

(defun my-chpl-mode-hook ()
 (my-common-mode-hook)
)

(defun my-format-compile-test ()
  (interactive)
  (toggle-read-only -1) ; Always writable
  (beginning-of-buffer)
  (replace-string "./" "start_test ")
  (beginning-of-buffer)
  (replace-regexp "\.....\.out\.tmp:1:" ".chpl")
)

(defun my-compilation-mode-hook ()
 (define-key compilation-mode-map "\C-c\C-t" 'my-format-compile-test)
)

(add-hook 'diff-mode-hook 'my-common-mode-hook)
(add-hook 'text-mode-hook 'my-text-mode-hook)
(add-hook 'c-mode-hook 'my-c-mode-hook)
(add-hook 'c++-mode-hook 'my-c++-mode-hook)
(add-hook 'chpl-mode-hook 'my-chpl-mode-hook)
(add-hook 'compilation-mode-hook 'my-compilation-mode-hook)

; Use text mode by default.
; (text-mode)

(split-window-horizontally)

; Start a shell
(shell nil)