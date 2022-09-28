:: Generic files
set generic=src\generic\array\array.c src\generic\map\map.c

:: Game files
set game=src\game\init.c
set state=src\game\state\loop.c src\game\state\game_global.c
set stores=src\game\state\stores\in_game_store.c
set state_unit_management=src\game\state\unit_management\unit_selection.c
set state_events=src\game\state\events\events.c src\game\state\events\ui_events.c src\game\state\stores\in_game_events.c src\game\state\events\mouse_event_handlers.c
set physics=src\game\state\physics\collisions.c
set game_constants=src\game\constants\maps.c
set game_entities=src\game\game_entities\game_entity.c
set game_units=src\game\game_entities\units\worker.c src\game\game_entities\units\resource.c src\game\game_entities\units\spike.c
set game_buildings=src\game\game_entities\buildings\building.c src\game\game_entities\buildings\rally_point.c
set game_core=src\game\game_entities\core\mouse_ray.c src\game\game_entities\core\drag_select.c src\game\game_entities\core\player.c
set ui_entities=src\game\game_entities\ui\command_board\command_board.c src\game\game_entities\ui\building\building_selection.c src\game\game_entities\ui\command_board\command_board_buttons.c src\game\game_entities\ui\player_ui\player_resources.c src\game\game_entities\ui\command_board\command_board_selection_icon.c src\game\game_entities\ui\command_board\command_board_info.c
set entity_components=src\game\game_entities\components\harvester_component.c src\game\game_entities\components\selector.c src\game\game_entities\components\building_component.c src\game\game_entities\components\resource_component.c src\game\game_entities\components\ui_component.c src\game\game_entities\components\selectable_component.c src\game\game_entities\components\combat_component.c src\game\game_entities\components\builder_component.c
set game_map=src\game\game_map\game_map.c

:: Engine Files
set camera=src\engine\camera\camera.c
set entity=src\engine\entity\entity.c
set render=src\engine\render\render.c src\engine\render\render_init.c src\engine\render\util\render_util.c src\engine\render\render_item\render_item.c src\engine\render\opengl_objects\opengl_objects.c
set font=src\engine\render\render_fonts\render_fonts.c
set io=src\engine\io\io.c
set time=src\engine\time\time.c
set config=src\engine\io\config.c
set input=src\engine\io\input.c
set util=src\engine\util\constants.c src\engine\util\util.c

:: Organizing files
set game_files=%game_constants% %game% %game_entities% %state% %state_events% %game_map% %game_core% %stores% %ui_entities% %game_buildings% %entity_components% %game_units% %state_unit_management% %physics%
set engine_files=%render% %io% %config% %time% %input% %util% %camera% %font% %entity%
set files=src\glad.c src\json.c src\main.c src\engine\global.c %generic% %engine_files% %game_files%
set libs=C:\Users\ivanr\Projects\engine\lib\SDL2main.lib C:\Users\ivanr\Projects\engine\lib\SDL2.lib C:\Users\ivanr\Projects\engine\lib\freetype.lib

:: Building executable
CL /Fo:C:\Users\ivanr\Projects\engine\build\ /Zi /I C:\Users\ivanr\Projects\engine\include %files% /link %libs% /OUT:game.exe