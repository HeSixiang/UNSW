$vscode_path = Get-Command code
If ([string]::IsNullOrEmpty($vscode_path)) {
   'Visual Studio Code not found. Please download and install Visual Studio Code from https://code.visualstudio.com/download.'
   exit 1
}

code --install-extension eamodio.gitlens                             && \
code --install-extension Equinusocio.vsc-community-material-theme    && \
code --install-extension Equinusocio.vsc-material-theme              && \
code --install-extension equinusocio.vsc-material-theme-icons        && \
code --install-extension GitHub.vscode-pull-request-github           && \
code --install-extension helgardrichard.helium-icon-theme            && \
code --install-extension j-zeppenfeld.tab-indent-space-align         && \
code --install-extension llvm-vs-code-extensions.vscode-clangd       && \
code --install-extension ms-vscode-remote.remote-ssh                 && \
code --install-extension ms-vscode-remote.remote-ssh-edit            && \
code --install-extension ms-vscode-remote.remote-wsl                 && \
code --install-extension ms-vscode.cmake-tools                       && \
code --install-extension twxs.cmake                                  && \
code --install-extension vadimcn.vscode-lldb                         && \
code --install-extension webfreak.debug
