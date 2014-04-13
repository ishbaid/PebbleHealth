	
#include <pebble.h>

Window *window;

TextLayer *text_layer;
TextLayer *response;


ScrollLayer *scroll_layer;

static char scroll_text[] = "Congratulations! You have successfully met your daily intake for vitamins! According to the National Institute of Health, the recommended intake for an average adult is 700-900 mcg for Vitamin A, 2.4 mcg for Vitamin B12, 82 mcg for Vitamin C, and 15 mcg for Vitamin D.";
static char needA[]="Your body may require additional Vitamin A. Increasing your intake of Vitamin A could vastly improve your eyesight as well as prevent cancer, age-related macular degeneration, and measles. Some foods that could help increase your Vitamin A intake are: meats, fish (such as salmon), vegetables (like broccoli and carrots), and dairy products.";
static char needB[]="Your body may require additional Vitamin B12, which is known to help make genetic material and prevent anemia. Increasing your B12 intake could greatly increase your day-to-day energy. Vitamin B12 deficiency can lead to tiredness, weakness, weight loss, and even depression. Some foods that could help increase your Vitamin B12 intake are: beef liver, fish, poultry, eggs, and dairy products.";
static char needC[] = "Your body may require additional Vitamin C, which is known to increase the strength of your immune system. It is rare to see Vitamin C deficiency in the United States; however, Vitamin C deficiency can lead to scurvy which can lead to anemia and eventually death. Some foods that could help increase your Vitamin C intake are: oranges (or other citrus food), strawberries, broccoli, and red/green peppers.";
static char needD[] = "Your body may require additional Vitamin D, which promotes calcium absorption. Having enough amounts of Vitamin D ensure strong bones and prevents hypocalcemic tetany. Vitamin D does not naturally preside in most foods. Some foods that do have Vitamin D includes: fish liver oils, flesh of fatty fish (salmon and tuna), and dairy products.";


static const int vert_scroll_text_padding = 4;

// The scroll layer can other things in it such as an invert layer
static InverterLayer *inverter_layer;

char *foods[] = {"Have you had your daily serving of...","2-3 oz Milk or other dairy products?", "3-4 servings Broccoli?", "Fish or fish oil?", "3-6 oz poultry?", "Eggs?", "Oranges or other citrus fruit?", "4 servings of other fruits?", "3- 4 servings Carrots or peppers?"};
char *answer[]={"Yes", "No", "Some"};
int counter = 0;
int rVal = 0;
int score[8];
char total[]= "00000000";
char values[] = "0123456789";
char *numbers[]={"one", "two", "three", "four", "five", "six", "seven", "eight", "nine", "ten", "ele", "twel"};
char *alpha[] = {"a","b","c","d","e","f","g","h","i","j","k","l","m","n","o","p","q","r","s","t","u","v","w","x","y","z"};
int a = 0;
int b = 0;
int c = 0;
int d = 0;
bool completed = 0;
int completed2 = 0;

//keeps track of weather or not each vitamin notice needs to be posted
bool printA = 0;
bool printB = 0;
bool printC = 0;
bool printD = 0;

//keeps track of whether or not all the notices have been printed
bool done = 0;
////////////////////////////////////////////////////
//keeps track of whether or not long has been pressed
int longPressed = 0;

Window *tips;

TextLayer *tip_text;

//prototypes
void set_up(void);
void long_config_provider(void *context);
void long_select_click_handler(ClickRecognizerRef recognizer, void *context);
void long_up_click_handler(ClickRecognizerRef recognizer, void *context);
void long_down_click_handler(ClickRecognizerRef recognizer, void *context);
static void long_scroll_click_config_provider(void *context);
void long_scroll_init();


/////
static char t1[] = "Exercise is vital in order to help strengthen your immune system and prevent various diseases. The average adult should generally exercise for 2.5 hours a week at a moderate level or 1.25 hours at a vigorous level.";
static char t2[] = "Sleep is a vital part of human growth and development. Infants require between 12 and 18 hours of sleep, toddlers require between 12 and 14, school-age children require between 10 and 11, teenagers require between 8.5 and 9.5, and adults require between 7 and 9.";
static char t3[] = "Sleep deprivation can increase likelihood of obesity, diabetes, and depression. In order to achieve better sleep, it is recommended to have a consistent sleep cycle. In addition, it is suggested to create calm, relaxing rituals before bed and ensure that your bedroom is dark and cool. Exercise is also expected to help you sleep.";
static char t4[] = "Recommended calorie intake for a person between 3 and 5 years old is 1200, between 6 and 10 years old is 1400, between 11 and 40 years old is 2400. ";

int tCounter = 0;

ScrollLayer *scroll_tip;



// The scroll layer can other things in it such as an invert layer
static InverterLayer *inverter_tip;
  
  

////////////////////////////////////////////////////


//prototype
void select_click_handler(ClickRecognizerRef recognizer, void *context);


// scroll_layer for when no vitamins are needed
void scroll_NoneNeeded(){
  
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_frame(window_layer);
  GRect max_text_bounds = GRect(0, 0, bounds.size.w, 2000);

  // Initialize the scroll layer
  scroll_layer = scroll_layer_create(bounds);

  // This binds the scroll layer to the window so that up and down map to scrolling
  // You may use scroll_layer_set_callbacks to add or override interactivity
  scroll_layer_set_click_config_onto_window(scroll_layer, window);

  // Initialize the text layer
  text_layer = text_layer_create(max_text_bounds);
  //A notice
  text_layer_set_text(text_layer, scroll_text);
  

  // Change the font to a nice readable one
  // This is system font; you can inspect pebble_fonts.h for all system fonts
  // or you can take a look at feature_custom_font to add your own font
  text_layer_set_font(text_layer, fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD));

  // Trim text layer and scroll content to fit text box
  GSize max_size = text_layer_get_content_size(text_layer);
  text_layer_set_size(text_layer, max_size);
  scroll_layer_set_content_size(scroll_layer, GSize(bounds.size.w, max_size.h + vert_scroll_text_padding));

  // Add the layers for display
  scroll_layer_add_child(scroll_layer, text_layer_get_layer(text_layer));

  // The inverter layer will highlight some text
  inverter_layer = inverter_layer_create(GRect(0, 28, bounds.size.w, 28));
  scroll_layer_add_child(scroll_layer, inverter_layer_get_layer(inverter_layer));

  layer_add_child(window_layer, scroll_layer_get_layer(scroll_layer));
  
}
  
static void scroll_click_config_provider(void *context){
  
  window_single_click_subscribe(BUTTON_ID_SELECT, select_click_handler);

}
ScrollLayerCallbacks slc = {.click_config_provider = scroll_click_config_provider};
//initializes scroll for vitamins needed
void scroll_init(){
  
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_frame(window_layer);
  GRect max_text_bounds = GRect(0, 0, bounds.size.w, 2000);

  // Initialize the scroll layer
  scroll_layer = scroll_layer_create(bounds);

  // This binds the scroll layer to the window so that up and down map to scrolling
  // You may use scroll_layer_set_callbacks to add or override interactivity
  //THE ORDER MATTERS OF WHICH FUNCTION IS CALLED FIRST
  scroll_layer_set_callbacks(scroll_layer, slc);
  scroll_layer_set_click_config_onto_window(scroll_layer, window);


  // Initialize the text layer
  text_layer = text_layer_create(max_text_bounds);
  //A notice
  if(printA == 1){
    
    text_layer_set_text(text_layer, needA);
    printA = 0;
  }
  else if(printB == 1){
    
    text_layer_set_text(text_layer, needB);
    printB = 0;
  }
  else if(printC == 1){
    
    text_layer_set_text(text_layer, needC);
    printC = 0;
  }
  else if(printD == 1){
    
    text_layer_set_text(text_layer, needD);
    printD = 0;
  }
  
  if(printA == 0 && printB == 0 && printC == 0 && printD == 0){
    
    done = 1;
  }

  // Change the font to a nice readable one
  // This is system font; you can inspect pebble_fonts.h for all system fonts
  // or you can take a look at feature_custom_font to add your own font
  text_layer_set_font(text_layer, fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD));

  // Trim text layer and scroll content to fit text box
  GSize max_size = text_layer_get_content_size(text_layer);
  text_layer_set_size(text_layer, max_size);
  scroll_layer_set_content_size(scroll_layer, GSize(bounds.size.w, max_size.h + vert_scroll_text_padding));

  // Add the layers for display
  scroll_layer_add_child(scroll_layer, text_layer_get_layer(text_layer));

  // The inverter layer will highlight some text
  inverter_layer = inverter_layer_create(GRect(0, 28, bounds.size.w, 28));
  scroll_layer_add_child(scroll_layer, inverter_layer_get_layer(inverter_layer));

  layer_add_child(window_layer, scroll_layer_get_layer(scroll_layer));
  
}

void removeAll(){
  
  

  text_layer_destroy(response);
}
//calculates vitamin points
void calculate(){
  completed = 1;
  text_layer_set_text(text_layer, "DONE!");
  for(int i = 0; i < 8; i ++){
    
    int j = score[i];
    total[i] = values[j];
  }
   //text_layer_set_text(response, total);
    //text_layer_destroy(response);
  //ISNT WORKING
  //layer_remove_from_parent((Layer*)&response);
  //milk
  if(score[0] == 0){
    
    a += 4;
    b += 5;
    d += 2;
  }
  else if (score[0] == 2){
    
    a += 2;
    b += 3;
    d += 1;
  }
  //brocolli
  if(score[1] == 0){
    
    a += 5;
    b += 1;
    c += 1;
  }
  else if (score[1] == 2){
    
    a += 3;
    b += 1;
    c += 1;
  }
  //fish
  if(score[2] == 0){
    
    a += 3;
    b += 2;
    d += 10;
  }
  else if (score[2] == 2){
    
    a += 2;
    b += 1;
    d += 5;
  }
  //meat
  if(score[3] == 0){
    
    a += 3;
    b += 5;
    
  }
  else if (score[3] == 2){
    
    a += 2;
    b += 3;

  }
  
  //eggs
  if(score[4] == 0){
    
    b += 2;

  }
  else if (score[4] == 2){
    
    b += 1;
  }
    //orange
  if(score[5] == 0){
    

    c += 10;
    d += 3;
  }
  else if (score[5] == 2){
    
    c += 5;
    d += 2;
  }
  
    //fruit
  if(score[6] == 0){
    
    c += 1;
  }
  else if (score[6] == 2){
    
      c += 1;
  }
  //carrots
  if(score[7] == 0){
    
    a += 3;
    c += 3;
    
  }
  else if (score[7] == 2){
    
    a += 2;
    c += 2;
  }
  //calculates if vitamins are needed
  if(a < 10)
    printA = 1;
  if(b < 10)
    printB = 1;
  if(c < 10)
    printC = 1;
  if(d < 10)
    printD = 1;
  //No vitamins are needed
  if(printA ==0 && printB == 0 && printC == 0 && printD == 0){
    

    removeAll();
    scroll_NoneNeeded();
    
  }
  
  //if any vitamins are needed
  if(printA == 1 || printB == 1 || printC == 1 || printD == 1){
    
    removeAll();
    scroll_init();
    
  }
  

  
      
  
}
//select
void select_click_handler(ClickRecognizerRef recognizer, void *context){
  
  //if number hasn't been entered, then
  if(completed == 0){
    
      
     if (counter <= 9){
    
 
        if(counter >= 2){
    
          
        //minus three because first 2 screens are instructions

           score[counter - 2] = rVal % 3;
           text_layer_set_text( response, *(answer));
        
         }

         text_layer_set_text( text_layer, *(foods + counter));
         rVal = 0;
         counter ++;
         if(counter == 2)
           layer_set_hidden(text_layer_get_layer(response), false);
    
    }
    if(counter == 10){
    
      calculate();
    }
  }
  else if(completed == 1 && done == 0){
    
    if(printB == 1){
      
      text_layer_set_text(text_layer, needB);
      printB = 0;
    }
    else if (printC == 1){
      
      text_layer_set_text(text_layer, needC);
      printC = 0;
    }
    else if (printD == 1){
      
      text_layer_set_text(text_layer, needD);
      printD = 0;
      done = 1;
    }
    
  }
  else{
    
      text_layer_set_text(text_layer, "Diagnosis Complete");

  }
  
  
}
//handles up button events
void up_click_handler(ClickRecognizerRef recognizer, void *context){
    
    if(rVal == 0)
        rVal = 2;
    else
      rVal --;
    text_layer_set_text( response, *(answer + rVal % 3));
      
}

//handles down button events
void down_click_handler(ClickRecognizerRef recognizer, void *context){

    rVal ++;
    text_layer_set_text( response, *(answer + rVal % 3));
}
////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
//long click
void select_long_click_handler(ClickRecognizerRef recognizer, void *context) {
 
  APP_LOG(APP_LOG_LEVEL_DEBUG, "Long click");
  longPressed = 1;
  set_up();
  Window *window = (Window *)context;
  
  
  // This context defaults to the window, but may be changed with \ref window_set_click_context.
}

//release
void select_long_click_release_handler(ClickRecognizerRef recognizer, void *context) {

    APP_LOG(APP_LOG_LEVEL_DEBUG, "release");
  Window *window = (Window *)context; // This context defaults to the window, but may be changed with \ref window_set_click_context.
}

//sets up tips window
void set_up(void) {
	// Create a window and text layer
	tips = window_create();
	tip_text = text_layer_create(GRect(0, 0, 144, 154));
  

	// Set the text, font, and text alignment
	text_layer_set_text(tip_text, "Life and Safety Tips ");
	text_layer_set_font(text_layer, fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD));
	text_layer_set_text_alignment(text_layer, GTextAlignmentCenter);
  

  

	// Add the text layer to the window
	layer_add_child(window_get_root_layer(tips), text_layer_get_layer(tip_text));

  //handles clicks
  window_set_click_config_provider(tips, long_config_provider);
  
	// Push the window
	window_stack_push(tips, true);

	// App Logging!
	APP_LOG(APP_LOG_LEVEL_DEBUG, "Pushing new window");
}

void long_config_provider(void *context){
  
  window_single_click_subscribe(BUTTON_ID_SELECT, long_select_click_handler);
  window_single_click_subscribe(BUTTON_ID_UP, long_up_click_handler);
  window_single_click_subscribe(BUTTON_ID_DOWN, long_down_click_handler);
  
}

void long_select_click_handler(ClickRecognizerRef recognizer, void *context){
  
  APP_LOG(APP_LOG_LEVEL_DEBUG, "Long select");
  long_scroll_init();
  
}
void long_up_click_handler(ClickRecognizerRef recognizer, void *context){
  
    APP_LOG(APP_LOG_LEVEL_DEBUG, "Long up");
  
}
void long_down_click_handler(ClickRecognizerRef recognizer, void *context){
  
  
    APP_LOG(APP_LOG_LEVEL_DEBUG, "Long down");
}

 
static void long_scroll_click_config_provider(void *context){
  
  window_single_click_subscribe(BUTTON_ID_SELECT, long_select_click_handler);

}
ScrollLayerCallbacks long_slc = {.click_config_provider = long_scroll_click_config_provider};
//initializes scroll for vitamins needed
void long_scroll_init(){

  APP_LOG(APP_LOG_LEVEL_DEBUG, "Scroll init for long");
  Layer *window_layer = window_get_root_layer(tips);
  GRect bounds = layer_get_frame(window_layer);
  GRect max_text_bounds = GRect(0, 0, bounds.size.w, 2000);

  // Initialize the scroll layer
  scroll_tip = scroll_layer_create(bounds);

  // This binds the scroll layer to the window so that up and down map to scrolling
  // You may use scroll_layer_set_callbacks to add or override interactivity
  //THE ORDER MATTERS OF WHICH FUNCTION IS CALLED FIRST
  scroll_layer_set_callbacks(scroll_tip, long_slc);
  scroll_layer_set_click_config_onto_window(scroll_tip, tips);


  // Initialize the text layer
  tip_text = text_layer_create(max_text_bounds);
  text_layer_set_text(tip_text, t1); 

  // Change the font to a nice readable one
  // This is system font; you can inspect pebble_fonts.h for all system fonts
  // or you can take a look at feature_custom_font to add your own font
  text_layer_set_font(tip_text, fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD));

  // Trim text layer and scroll content to fit text box
  GSize max_size = text_layer_get_content_size(tip_text);
  text_layer_set_size(tip_text, max_size);
  scroll_layer_set_content_size(scroll_tip, GSize(bounds.size.w, max_size.h + vert_scroll_text_padding));

  // Add the layers for display
  scroll_layer_add_child(scroll_tip, text_layer_get_layer(tip_text));

  // The inverter layer will highlight some text
  inverter_tip = inverter_layer_create(GRect(0, 28, bounds.size.w, 28));
  scroll_layer_add_child(scroll_tip, inverter_layer_get_layer(inverter_tip));

  layer_add_child(window_layer, scroll_layer_get_layer(scroll_tip));
  
}


////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////

//delegates functions to handle the different button presses
void config_provider(void *context){
  
  window_single_click_subscribe(BUTTON_ID_SELECT, select_click_handler);
  window_single_click_subscribe(BUTTON_ID_UP, up_click_handler);
  window_single_click_subscribe(BUTTON_ID_DOWN, down_click_handler);
  
  window_long_click_subscribe(BUTTON_ID_SELECT, 700, select_long_click_handler, select_long_click_release_handler);
  
}

//initializes window
void handle_init(void) {
	// Create a window and text layer
	window = window_create();
	text_layer = text_layer_create(GRect(0, 0, 144, 154));
  response = text_layer_create(GRect(50, 100, 100, 40));

	// Set the text, font, and text alignment
	text_layer_set_text(text_layer, "Welcome to Pebble Health!\n Press Select to begin ");
	text_layer_set_font(text_layer, fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD));
	text_layer_set_text_alignment(text_layer, GTextAlignmentCenter);
  
  //RESPONSE
  text_layer_set_text(response, "Yes");
	text_layer_set_font(response, fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD));
	text_layer_set_text_alignment(response, GTextAlignmentCenter);
  layer_set_hidden(text_layer_get_layer(response), true);
  

	// Add the text layer to the window
	layer_add_child(window_get_root_layer(window), text_layer_get_layer(text_layer));
	layer_add_child(window_get_root_layer(window), text_layer_get_layer(response));
  //handles clicks
  window_set_click_config_provider(window, config_provider);
  
	// Push the window
	window_stack_push(window, true);

	// App Logging!
	APP_LOG(APP_LOG_LEVEL_DEBUG, "Hello World");
}

//deallocates space
void handle_deinit(void) {
	// Destroy the text layer
	text_layer_destroy(text_layer);
  
  text_layer_destroy(response);
  inverter_layer_destroy(inverter_layer);

  scroll_layer_destroy(scroll_layer);

	// Destroy the window
	window_destroy(window);
  
  
  text_layer_destroy(tip_text);
  window_destroy(tips);
  
}

int main(void) {
	handle_init();
	app_event_loop();
	handle_deinit();
}