import React, { useState, useCallback } from 'react';
import { Ice } from '@zeroc/ice';
import { VisitorCenter } from '../Greeter.js';

interface AppState {
  name: string;
  greeting: string;
  loading: boolean;
  error: string;
}

function App(): JSX.Element {
  const [state, setState] = useState<AppState>({
    name: 'React',
    greeting: '',
    loading: false,
    error: ''
  });

  const sendGreeting = useCallback(async () => {
    if (!state.name.trim()) {
      setState(prev => ({ ...prev, error: 'Please enter a name' }));
      return;
    }

    setState(prev => ({ ...prev, loading: true, error: '', greeting: '' }));

    try {
      // Create an Ice communicator
      const communicator = Ice.initialize([]);
      
      // Create a proxy to the Greeter object
      // If you run the server on a different computer, replace localhost with the server's hostname or IP address
      const greeter = new VisitorCenter.GreeterPrx(communicator, 'greeter:tcp -h localhost -p 4061');
      
      // Send the greeting request
      const greeting = await greeter.greet(state.name.trim());
      
      setState(prev => ({ ...prev, greeting, loading: false }));
      
      // Clean up the communicator
      communicator.destroy();
    } catch (error) {
      console.error('Error sending greeting:', error);
      setState(prev => ({ 
        ...prev, 
        error: `Failed to send greeting: ${error instanceof Error ? error.message : 'Unknown error'}`,
        loading: false 
      }));
    }
  }, [state.name]);

  const handleNameChange = (event: React.ChangeEvent<HTMLInputElement>) => {
    setState(prev => ({ ...prev, name: event.target.value }));
  };

  const handleSubmit = (event: React.FormEvent<HTMLFormElement>) => {
    event.preventDefault();
    sendGreeting();
  };

  return (
    <div className="container">
      <h1>Ice Greeter React Demo</h1>
      <p>
        This demo shows how to use Ice for JavaScript in a React application.
        Make sure you have a Greeter server running on localhost:4061.
      </p>
      
      <form className="greeting-form" onSubmit={handleSubmit}>
        <div className="form-group">
          <label htmlFor="name">Name:</label>
          <input
            id="name"
            type="text"
            value={state.name}
            onChange={handleNameChange}
            placeholder="Enter your name"
            disabled={state.loading}
          />
        </div>
        
        <button type="submit" disabled={state.loading}>
          {state.loading ? 'Sending...' : 'Send Greeting'}
        </button>
      </form>

      {state.loading && (
        <div className="response loading">
          Sending greeting request...
        </div>
      )}

      {state.error && (
        <div className="response error">
          {state.error}
        </div>
      )}

      {state.greeting && (
        <div className="response success">
          <strong>Server Response:</strong> {state.greeting}
        </div>
      )}
    </div>
  );
}

export default App;