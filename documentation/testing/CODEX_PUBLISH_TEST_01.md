# Prompt para publicar a etapa 01 com o Codex local

A preparação foi concluída, mas a integração GitHub retornou HTTP 403,
`Resource not accessible by integration`, ao tentar criar a árvore do commit.
Não houve commit, branch remota nem PR publicados por esta sessão.
A branch `test/solarview-async-validation-01` existe apenas no checkout de trabalho.
Use este prompt no Codex que tenha acesso autorizado ao seu repositório.

---

Publique a etapa SolarView 01 da InverterModbusLib. O usuário autorizou reservar
este trabalho no repositório; não faça merge na branch de desenvolvimento.

1. Localize o ZIP `InverterModbusLib-SolarView-Teste01.zip` que entreguei ao
   usuário. Leia seu `LEIA_PRIMEIRO.md`, o roteiro, a revisão e a verificação em
   `InverterModbusLib/documentation/testing/`. Não migre nenhum comando novo.
2. Preserve integralmente mudanças locais, especialmente o antigo `src/main.cpp`.
   Use um clone separado de `https://github.com/akioShaolin/InverterModbusLib.git`.
   Leia quaisquer instruções do repositório. Confira a identidade Git configurada;
   não invente autor nem exponha credenciais.
3. A base exata do patch é `eaa6006f287e531232f0aec220389c497654eb33`, da branch
   `refactor/nonblocking-modbus`. Crie `test/solarview-async-validation-01` a partir
   dessa base, após conferir se a branch remota já existe. Se existir, compare
   o conteúdo e prossiga sem sobrescrever trabalho posterior nem usar force push.
4. No clone limpo, rode `git apply --check` com `solarview-async-01.patch`, da raiz
   do ZIP, e depois aplique-o. O patch contém todos os arquivos novos; não copie
   primeiro a árvore extraída sobre o clone. Se a base remota avançou, mantenha
   a base de ensaio documentada e relate a diferença; não faça rebase silencioso.
5. Confira o diff. Compile com `pio run -e esp07`. Execute as regressões host
   documentadas quando houver bash/g++ disponíveis; se não houver, informe a
   limitação e preserve o relatório já entregue. Não alegue teste físico.
6. Atualize apenas as notas de publicação do roteiro e deste arquivo para o
   estado efetivamente obtido. Faça commit das mudanças desta etapa com uma
   descrição clara e envie somente esta branch, usando a autenticação já
   autorizada. Se faltar permissão, informe o erro sem alterar controles de acesso.
7. Entregue URL da branch, SHA do commit e resultado do build. Um PR de rascunho
   para `refactor/nonblocking-modbus` é aceitável; não faça merge. Não suba `.pio`,
   binários, CSVs reais com seriais ou credenciais. O snapshot textual do main
   recebido e os testes host fazem parte da mudança.

O escopo é publicação, não continuação da migração. `CODEX_NEXT_COMMAND.md`
só será usado depois que o usuário retornar os logs e escolher a etapa seguinte.
