import { useState } from 'react'

import SpotForm from './SpotForm'

import type { Meta, StoryObj } from '@storybook/react'

// More on how to set up stories at: https://storybook.js.org/docs/react/writing-stories/introduction#default-export
const meta = {
    // More on argTypes: https://storybook.js.org/docs/react/api/argtypes
    argTypes: {},
    component: SpotForm,
    parameters: {
        // Optional parameter to center the component in the Canvas. More info: https://storybook.js.org/docs/react/configure/story-layout
        layout: 'centered',
    },
    // This component will have an automatically generated Autodocs entry: https://storybook.js.org/docs/react/writing-docs/autodocs
    tags: ['autodocs'],
} satisfies Meta<typeof SpotForm>

export default meta
type Story = StoryObj<typeof meta>

export const Default: Story = {
    args: {},
    render: (args) => {
        const [values, setValues] = useState([''])
        return (
            <div className='space-y-4'>
                <p>
                    values: <code>{JSON.stringify(values)}</code>
                </p>
                <SpotForm
                    values={values}
                    onChange={(v) => setValues(v)}
                    {...args}
                />
            </div>
        )
    },
}