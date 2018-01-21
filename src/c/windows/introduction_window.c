#include "introduction_window.h"

static Window *introWindow;
static int predicted_happiness, predicted_activation;
static bool hasMachineLearning = false, canProceedToMood = false, hasInternetConnection = false, hasReceivedResult = false;
static TextLayer *tree_text_layer;
static TextLayer *heading_text_layer;
static TextLayer *machine_learning_text_layer;
static GBitmap *smileyImage, *activationImage, *happinessImage;
static BitmapLayer *smileyImageLayer, *activationImageLayer, *happinessImageLayer;
static ActionBarLayer *s_action_bar_layer;
static GBitmap *s_tick_bitmap, *s_cross_bitmap, *s_go_bitmap, *s_zz_bitmap, *s_social_bitmap;

/***********************************
* Click handler functions          *
***********************************/
void intro_up_click_handler(ClickRecognizerRef recognizer, void *context){
  if(launch_reason() == APP_LAUNCH_WAKEUP) {  
    delay_timer();
    window_stack_pop_all(true);
  } else {
    if(hasInternetConnection) {
      request_friends();
      window_stack_push(friends_window_get_window(), true); // show the friends window
    }
    else {
      window_stack_push(introduction_window_get_window(), true);
    }
  }
}

void intro_down_click_handler(ClickRecognizerRef recognizer, void *context){
  if(hasMachineLearning) {
    window_stack_push(happiness_input_window_get_window(), true); // show the main window
  }
  else {
     window_stack_pop_all(true);
  }
}

void intro_select_click_handler(ClickRecognizerRef recognizer, void *context){
  if(hasMachineLearning) {
    setHappiness(predicted_happiness);
    setActivation(predicted_activation);
    window_stack_push(creativity_input_window_get_next_window(), true);
  } else if(canProceedToMood) {
    window_stack_push(happiness_input_window_get_window(), true); // show the main window
  } 
}

void intro_back_click_handler(ClickRecognizerRef recognizer, void *context){
  window_stack_pop_all(true);
}

/***********************************
* Right buttons click config       *
***********************************/
void intro_click_config_provider(void *context){
  window_single_click_subscribe(BUTTON_ID_UP, (ClickHandler)intro_up_click_handler);
  window_single_click_subscribe(BUTTON_ID_DOWN, (ClickHandler)intro_down_click_handler);
  window_single_click_subscribe(BUTTON_ID_SELECT, (ClickHandler)intro_select_click_handler);
  window_single_click_subscribe(BUTTON_ID_BACK, (ClickHandler)intro_back_click_handler);
}

/***********************************
* Sets the text of the intro window*
***********************************/
void set_mood_window_text(int happiness, int activation) {
  if(hasReceivedResult) {
    return;
  }
  
  action_bar_layer_clear_icon(s_action_bar_layer, BUTTON_ID_UP);
  action_bar_layer_clear_icon(s_action_bar_layer, BUTTON_ID_DOWN);
  action_bar_layer_clear_icon(s_action_bar_layer, BUTTON_ID_SELECT);
  
  if(launch_reason() == APP_LAUNCH_WAKEUP) {
    action_bar_layer_set_icon(s_action_bar_layer, BUTTON_ID_UP, s_zz_bitmap);
  }
  
  if(happiness == -1 && activation == -1) {
    // mood has not been loaded yet
    hasInternetConnection = false;
    smileyImage = gbitmap_create_with_resource(RESOURCE_ID_loadingscreen_144x100);
    text_layer_set_text(machine_learning_text_layer, "Loading...");
  } else if(happiness == -2 && activation == -2) {
    // there is no trained model yet
    hasReceivedResult = true;
    canProceedToMood = true;
    hasInternetConnection = true;
    hasMachineLearning = false;
    smileyImage = gbitmap_create_with_resource(RESOURCE_ID_noMachieneLearning_144x100);
    text_layer_set_text(machine_learning_text_layer, "More training data is needed.");
    action_bar_layer_set_icon(s_action_bar_layer, BUTTON_ID_SELECT, s_go_bitmap);
    
    if(launch_reason() != APP_LAUNCH_WAKEUP) {
      action_bar_layer_set_icon(s_action_bar_layer, BUTTON_ID_UP, s_social_bitmap);
    }
  } else if(happiness == -3 && activation == -3) {
    // there is no trained model yet
    // no bluetooth connection
    hasReceivedResult = true;
    canProceedToMood = true;
    
    hasInternetConnection = false;
    hasMachineLearning = false;
    
    smileyImage = gbitmap_create_with_resource(RESOURCE_ID_NO_CONNECTION);
    action_bar_layer_set_icon(s_action_bar_layer, BUTTON_ID_DOWN, s_cross_bitmap);
    action_bar_layer_set_icon(s_action_bar_layer, BUTTON_ID_SELECT, s_tick_bitmap);
    text_layer_set_text(machine_learning_text_layer, "No connection to the phone. Continue?");
    
    
     // change the frame of the smiley image layer
    predicted_happiness = 0;
    predicted_activation = 0;
    Layer *window_layer = window_get_root_layer(introWindow);
    GRect bounds = layer_get_bounds(window_layer);
    GRect newBounds = GRect(-ACTION_BAR_WIDTH / 2, 15, 144 - ACTION_BAR_WIDTH, 100);
    layer_set_frame(bitmap_layer_get_layer(smileyImageLayer), newBounds);
    
    if(activation == 0) {
      if(happiness == 0) {
        smileyImage = gbitmap_create_with_resource(RESOURCE_ID_a_0_h_0);
        activationImage = gbitmap_create_with_resource(RESOURCE_ID_activation_0_small);
        happinessImage = gbitmap_create_with_resource(RESOURCE_ID_happiness_0_small);
      } else if(happiness == 1) {
        smileyImage = gbitmap_create_with_resource(RESOURCE_ID_a_0_h_1);
        activationImage = gbitmap_create_with_resource(RESOURCE_ID_activation_0_small);
        happinessImage = gbitmap_create_with_resource(RESOURCE_ID_happiness_1_small);
      } else if(happiness == 2) {
        smileyImage = gbitmap_create_with_resource(RESOURCE_ID_a_0_h_2);
        activationImage = gbitmap_create_with_resource(RESOURCE_ID_activation_0_small);
        happinessImage = gbitmap_create_with_resource(RESOURCE_ID_happiness_2_small);
      }
    } else if(activation == 1) {
      if(happiness == 0) {
        smileyImage = gbitmap_create_with_resource(RESOURCE_ID_a_1_h_0);
        activationImage = gbitmap_create_with_resource(RESOURCE_ID_activation_1_small);
        happinessImage = gbitmap_create_with_resource(RESOURCE_ID_happiness_0_small);
      } else if(happiness == 1) {
        smileyImage = gbitmap_create_with_resource(RESOURCE_ID_a_1_h_1);
        activationImage = gbitmap_create_with_resource(RESOURCE_ID_activation_1_small);
        happinessImage = gbitmap_create_with_resource(RESOURCE_ID_happiness_1_small);
      } else if(happiness == 2) {
        smileyImage = gbitmap_create_with_resource(RESOURCE_ID_a_1_h_2);
        activationImage = gbitmap_create_with_resource(RESOURCE_ID_activation_1_small);
        happinessImage = gbitmap_create_with_resource(RESOURCE_ID_happiness_2_small);
      }
    } else if(activation == 2) {
      if(happiness == 0) {
        smileyImage = gbitmap_create_with_resource(RESOURCE_ID_a_2_h_0);
        activationImage = gbitmap_create_with_resource(RESOURCE_ID_activation_2_small);
        happinessImage = gbitmap_create_with_resource(RESOURCE_ID_happiness_0_small);
      } else if(happiness == 1) {
        smileyImage = gbitmap_create_with_resource(RESOURCE_ID_a_2_h_1);
        activationImage = gbitmap_create_with_resource(RESOURCE_ID_activation_2_small);
        happinessImage = gbitmap_create_with_resource(RESOURCE_ID_happiness_1_small);
      } else if(happiness == 2) {
        smileyImage = gbitmap_create_with_resource(RESOURCE_ID_a_2_h_2);
        activationImage = gbitmap_create_with_resource(RESOURCE_ID_activation_2_small);
        happinessImage = gbitmap_create_with_resource(RESOURCE_ID_happiness_2_small);
      }
    }
  } else {
    hasReceivedResult = true;
    hasInternetConnection = true;
    predicted_happiness = happiness;
    predicted_activation = activation;
    hasMachineLearning = true;
    canProceedToMood = false;
    action_bar_layer_set_icon(s_action_bar_layer, BUTTON_ID_SELECT, s_tick_bitmap);
    action_bar_layer_set_icon(s_action_bar_layer, BUTTON_ID_DOWN, s_cross_bitmap);
    text_layer_set_text(machine_learning_text_layer, "Is this your current mood?");
    
    if(launch_reason() != APP_LAUNCH_WAKEUP) {
      action_bar_layer_set_icon(s_action_bar_layer, BUTTON_ID_UP, s_social_bitmap);
    }
    
    // change the frame of the smiley image layer
    Layer *window_layer = window_get_root_layer(introWindow);
    GRect bounds = layer_get_bounds(window_layer);
    GRect newBounds = GRect(-ACTION_BAR_WIDTH / 2, 15, 144 - ACTION_BAR_WIDTH, 100);
    layer_set_frame(bitmap_layer_get_layer(smileyImageLayer), newBounds);
    
    if(activation == 0) {
      if(happiness == 0) {
        smileyImage = gbitmap_create_with_resource(RESOURCE_ID_a_0_h_0);
        activationImage = gbitmap_create_with_resource(RESOURCE_ID_activation_0_small);
        happinessImage = gbitmap_create_with_resource(RESOURCE_ID_happiness_0_small);
      } else if(happiness == 1) {
        smileyImage = gbitmap_create_with_resource(RESOURCE_ID_a_0_h_1);
        activationImage = gbitmap_create_with_resource(RESOURCE_ID_activation_0_small);
        happinessImage = gbitmap_create_with_resource(RESOURCE_ID_happiness_1_small);
      } else if(happiness == 2) {
        smileyImage = gbitmap_create_with_resource(RESOURCE_ID_a_0_h_2);
        activationImage = gbitmap_create_with_resource(RESOURCE_ID_activation_0_small);
        happinessImage = gbitmap_create_with_resource(RESOURCE_ID_happiness_2_small);
      }
    } else if(activation == 1) {
      if(happiness == 0) {
        smileyImage = gbitmap_create_with_resource(RESOURCE_ID_a_1_h_0);
        activationImage = gbitmap_create_with_resource(RESOURCE_ID_activation_1_small);
        happinessImage = gbitmap_create_with_resource(RESOURCE_ID_happiness_0_small);
      } else if(happiness == 1) {
        smileyImage = gbitmap_create_with_resource(RESOURCE_ID_a_1_h_1);
        activationImage = gbitmap_create_with_resource(RESOURCE_ID_activation_1_small);
        happinessImage = gbitmap_create_with_resource(RESOURCE_ID_happiness_1_small);
      } else if(happiness == 2) {
        smileyImage = gbitmap_create_with_resource(RESOURCE_ID_a_1_h_2);
        activationImage = gbitmap_create_with_resource(RESOURCE_ID_activation_1_small);
        happinessImage = gbitmap_create_with_resource(RESOURCE_ID_happiness_2_small);
      }
    } else if(activation == 2) {
      if(happiness == 0) {
        smileyImage = gbitmap_create_with_resource(RESOURCE_ID_a_2_h_0);
        activationImage = gbitmap_create_with_resource(RESOURCE_ID_activation_2_small);
        happinessImage = gbitmap_create_with_resource(RESOURCE_ID_happiness_0_small);
      } else if(happiness == 1) {
        smileyImage = gbitmap_create_with_resource(RESOURCE_ID_a_2_h_1);
        activationImage = gbitmap_create_with_resource(RESOURCE_ID_activation_2_small);
        happinessImage = gbitmap_create_with_resource(RESOURCE_ID_happiness_1_small);
      } else if(happiness == 2) {
        smileyImage = gbitmap_create_with_resource(RESOURCE_ID_a_2_h_2);
        activationImage = gbitmap_create_with_resource(RESOURCE_ID_activation_2_small);
        happinessImage = gbitmap_create_with_resource(RESOURCE_ID_happiness_2_small);
      }
    }
  }
  
  bitmap_layer_set_bitmap(smileyImageLayer, smileyImage);
  bitmap_layer_set_bitmap(activationImageLayer, activationImage);
  bitmap_layer_set_bitmap(happinessImageLayer, happinessImage);
  layer_mark_dirty(bitmap_layer_get_layer(smileyImageLayer));
  layer_mark_dirty(bitmap_layer_get_layer(activationImageLayer));
  layer_mark_dirty(bitmap_layer_get_layer(happinessImageLayer));
  layer_mark_dirty(action_bar_layer_get_layer(s_action_bar_layer));
}

/***********************************
* Show warning message if receiving*
* the mood takes too long          *
* Reasons: phone not connected,    *
* no internet connection           *
***********************************/
void message_timeout_callback(void *data) {
  set_mood_window_text(-3, -3); // this means no internet connection
}

/***********************************
* Load event of the window         *
***********************************/
void introduction_window_load(Window *window){

  Layer *window_layer = window_get_root_layer(window);

  #ifndef PBL_SDK_3
    window_set_fullscreen(window, true);
  #endif

  GRect bounds = layer_get_bounds(window_layer);
  const GEdgeInsets label_insets = {.top = 5, .right = ACTION_BAR_WIDTH + 5, .left = 5, .bottom = 5};
  
  // add the smiley image
  smileyImageLayer = bitmap_layer_create(GRect(-ACTION_BAR_WIDTH / 2, 20, 144 - ACTION_BAR_WIDTH, 120));
  bitmap_layer_set_compositing_mode(smileyImageLayer, GCompOpSet);
  layer_add_child(window_layer, bitmap_layer_get_layer(smileyImageLayer));
  
  happinessImageLayer = bitmap_layer_create(GRect(0, 65, (144 - ACTION_BAR_WIDTH) / 2, 90));
  bitmap_layer_set_compositing_mode(happinessImageLayer, GCompOpSet);
  layer_add_child(window_layer, bitmap_layer_get_layer(happinessImageLayer));
  
  activationImageLayer = bitmap_layer_create(GRect((144 - ACTION_BAR_WIDTH) / 2, 65, (144 - ACTION_BAR_WIDTH) / 2, 90));
  bitmap_layer_set_compositing_mode(activationImageLayer, GCompOpSet);
  layer_add_child(window_layer, bitmap_layer_get_layer(activationImageLayer));
  
  // add the heading
  heading_text_layer = text_layer_create(grect_inset(bounds, label_insets));
  text_layer_set_background_color(heading_text_layer, GColorClear);
  text_layer_set_text_alignment(heading_text_layer, GTextAlignmentCenter);
  text_layer_set_font(heading_text_layer, fonts_get_system_font(FONT_KEY_GOTHIC_14_BOLD));
  text_layer_set_text(heading_text_layer, "Mood Prediction");
  layer_add_child(window_layer, text_layer_get_layer(heading_text_layer));
  
  // add the tree counter
  machine_learning_text_layer = text_layer_create(grect_inset(bounds, label_insets));
  text_layer_set_background_color(machine_learning_text_layer, GColorClear);
  text_layer_set_text_alignment(machine_learning_text_layer, GTextAlignmentCenter);
  text_layer_set_font(machine_learning_text_layer, fonts_get_system_font(FONT_KEY_GOTHIC_14));
  text_layer_set_text(machine_learning_text_layer, "Loading..");
  GRect frame = layer_get_frame(text_layer_get_layer(machine_learning_text_layer));
  GSize content = text_layer_get_content_size(machine_learning_text_layer);
  layer_set_frame(text_layer_get_layer(machine_learning_text_layer), GRect(frame.origin.x, frame.origin.y + (frame.size.h - (content.h * 2) - 5), frame.size.w, content.h * 2 + 5));
  layer_add_child(window_layer, text_layer_get_layer(machine_learning_text_layer));
  
  // add the action menu
  s_tick_bitmap = gbitmap_create_with_resource(RESOURCE_ID_TICK);
  s_cross_bitmap = gbitmap_create_with_resource(RESOURCE_ID_CROSS);
  s_go_bitmap = gbitmap_create_with_resource(RESOURCE_ID_NEXT);
  s_zz_bitmap = gbitmap_create_with_resource(RESOURCE_ID_SLEEP);
  s_social_bitmap = gbitmap_create_with_resource(RESOURCE_ID_SOCIAL);
  s_action_bar_layer = action_bar_layer_create();
  action_bar_layer_add_to_window(s_action_bar_layer, window);

  //set click handler
  window_set_click_config_provider(window, intro_click_config_provider);
  
  if(!hasReceivedResult) {
    set_mood_window_text(-1, -1); // -1 should mean still loading..
    
    // set a timer to show a warning message if receiving the mood takes to long
    app_timer_register(10000, message_timeout_callback, NULL);
  }
}

/***********************************
* Unload event of the window       *
***********************************/
void introduction_window_unload(){
  bitmap_layer_destroy(smileyImageLayer);
  bitmap_layer_destroy(activationImageLayer);
  bitmap_layer_destroy(happinessImageLayer);
  gbitmap_destroy(smileyImage);
  gbitmap_destroy(happinessImage);
  gbitmap_destroy(activationImage);
  gbitmap_destroy(s_tick_bitmap);
  gbitmap_destroy(s_cross_bitmap);
  gbitmap_destroy(s_go_bitmap);
  gbitmap_destroy(s_zz_bitmap);
  gbitmap_destroy(s_social_bitmap);
  action_bar_layer_destroy(s_action_bar_layer);
}

/***********************************
* Init the window          *
***********************************/
void init_introduction_window(void) {
  introWindow = window_create();
  window_set_background_color(introWindow, GColorWhite);
  window_set_window_handlers(introWindow, (WindowHandlers){.load = introduction_window_load, .unload = introduction_window_unload});
}

/***********************************
* Deinit the window         *
***********************************/
void deinit_introduction_window(void) {
  text_layer_destroy(tree_text_layer);
  window_destroy(introWindow);
}

/***********************************
* Returns the window handle        *
***********************************/
Window *introduction_window_get_window(void) {
  return introWindow;
}
