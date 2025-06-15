"use client";

import { useState, useEffect } from 'react';
import { useRouter } from 'next/navigation';
import { formatPhoneDisplay, formatPhoneForAPI, validatePhone } from '@/utils/phoneFormatter';
import Link from "next/link";

export default function SeniorAdminProfile() {
    const [loading, setLoading] = useState(true);
    const [profile, setProfile] = useState(null);
    const [error, setError] = useState("");
    const router = useRouter();

    // Состояние для формы редактирования
    const [isEditing, setIsEditing] = useState(false);
    const [formData, setFormData] = useState({
        lastName: "",
        firstName: "",
        patronymic: "",
        phone: "",
        currentPassword: "",
        newPassword: "",
        newPasswordRepeat: ""
    });
    const [updateError, setUpdateError] = useState("");
    const [updateSuccess, setUpdateSuccess] = useState("");

    useEffect(() => {
        // Проверка аутентификации
        const isLoggedIn = localStorage.getItem('isLoggedIn');
        const userData = JSON.parse(localStorage.getItem('medSchedulerUser') || '{}');

        if (!isLoggedIn || !userData.userId || userData.userType !== 'senior') {
            router.push('/login');
            return;
        }

        // Загружаем данные профиля из API
        const fetchProfile = async () => {
            try {
                const response = await fetch(`https://api.medscheduler.ru/get_profile_by_id?user_id=${userData.userId}`);
                const data = await response.json();                if (response.ok && data.success) {
                    // Устанавливаем данные профиля
                    const profileData = {
                        userId: userData.userId,
                        firstName: data.user.first_name || "",
                        lastName: data.user.last_name || "",
                        patronymic: data.user.patronymic || "",
                        phone: data.user.phone || userData.phone
                    };

                    setProfile(profileData);

                    // Инициализация полей формы редактирования
                    setFormData({
                        lastName: profileData.lastName,
                        firstName: profileData.firstName,
                        patronymic: profileData.patronymic,
                        phone: profileData.phone,
                        currentPassword: "",
                        newPassword: "",
                        newPasswordRepeat: ""
                    });
                } else {
                    throw new Error(data.error || "Не удалось получить данные профиля");
                }
            } catch (err) {
                console.error("Error fetching profile:", err);
                // В случае ошибки используем базовые данные из localStorage
                const profileData = {
                    userId: userData.userId,
                    firstName: "",
                    lastName: "",
                    patronymic: "",
                    phone: userData.phone || ""
                };

                setProfile(profileData);
                setFormData({
                    lastName: profileData.lastName,
                    firstName: profileData.firstName,
                    patronymic: profileData.patronymic,
                    phone: profileData.phone,
                    currentPassword: "",
                    newPassword: "",
                    newPasswordRepeat: ""
                });

                setError("Не удалось загрузить полные данные профиля. Показана базовая информация.");
            } finally {
                setLoading(false);
            }
        };

        fetchProfile();
    }, [router]);

    const handleEditToggle = () => {
        setIsEditing(!isEditing);
        setFormData({
            lastName: profile.lastName,
            firstName: profile.firstName,
            patronymic: profile.patronymic,
            phone: profile.phone,
            currentPassword: "",
            newPassword: "",
            newPasswordRepeat: ""
        });
        setUpdateError("");
        setUpdateSuccess("");
    };

    const handleChange = (e) => {
        const { name, value } = e.target;
        setFormData((prevData) => ({
            ...prevData,
            [name]: value
        }));
    };

    const handleSave = async () => {
        setLoading(true);
        setError(null);
        setUpdateSuccess(null);

        if (formData.phone && !validatePhone(formData.phone)) {
            setError("Введите корректный номер телефона.");
            setLoading(false);
            return;
        }

        const payload = {
            userId: profile.userId,
            lastName: formData.lastName,
            firstName: formData.firstName,
            patronymic: formData.patronymic,
            phone: formData.phone ? formatPhoneForAPI(formData.phone) : undefined,
        };

        if (formData.newPassword) {
            payload.newPassword = formData.newPassword;
            payload.newPasswordRepeat = formData.newPasswordRepeat;
        }

        try {
            const response = await fetch('https://api.medscheduler.ru/edit_senior_admin_profile', {
                method: 'PATCH',
                headers: { 'Content-Type': 'application/json' },
                body: JSON.stringify(payload),
            });

            const data = await response.json();

            if (response.ok && data.success) {
                setUpdateSuccess("Профиль успешно обновлен");

                // Обновляем данные профиля локально
                setProfile({
                    ...profile,
                    lastName: formData.lastName,
                    firstName: formData.firstName,
                    patronymic: formData.patronymic,
                    phone: formData.phone
                });

                // Обновляем в localStorage, если телефон изменился
                if (formData.phone !== profile.phone) {
                    const userData = JSON.parse(localStorage.getItem('medSchedulerUser') || '{}');
                    userData.phone = formData.phone;
                    localStorage.setItem('medSchedulerUser', JSON.stringify(userData));
                }

                // Сбрасываем поля паролей
                setFormData((prevData) => ({
                    ...prevData,
                    currentPassword: "",
                    newPassword: "",
                    newPasswordRepeat: ""
                }));

                // Выходим из режима редактирования через 2 секунды
                setTimeout(() => {
                    setIsEditing(false);
                    setUpdateSuccess("");
                }, 10000);
            } else {
                setUpdateError(data.error || "Ошибка обновления профиля");
            }
        } catch (err) {
            console.error("Profile update error:", err);
            setUpdateError("Не удалось подключиться к серверу");
        } finally {
            setLoading(false);
        }
    };

    if (loading) {
        return (
            <div className="min-h-[calc(80vh-theme(space.36))] flex items-center justify-center">
                <div className="text-xl text-gray-600">Загрузка профиля...</div>
            </div>
        );
    }

    return (
        <div className="max-w-4xl mx-auto py-8 px-4">
            <h1 className="text-3xl font-bold text-main2 mb-8">Профиль старшего администратора</h1>

            {error && (
                <div className="bg-red-100 border border-red-400 text-red-700 px-4 py-3 rounded mb-4">
                    {error}
                </div>
            )}

            <div className="bg-white shadow-md rounded-lg p-6 mb-6">
                <div className="flex flex-col md:flex-row justify-between md:items-center mb-6">
                    <h2 className="text-xl font-semibold text-main2">Личные данные</h2>
                    {!isEditing && (
                        <button
                            onClick={handleEditToggle}
                            className="mt-4 md:mt-0 px-4 py-2 bg-main text-white rounded hover:bg-main2 transition-colors"
                        >
                            Редактировать профиль
                        </button>
                    )}
                </div>

                {!isEditing ? (
                    <div className="grid grid-cols-1 md:grid-cols-2 gap-4">
                        <div>
                            <p className="text-sm text-gray-500">Фамилия</p>
                            <p className="text-lg">{profile.lastName}</p>
                        </div>
                        <div>
                            <p className="text-sm text-gray-500">Имя</p>
                            <p className="text-lg">{profile.firstName}</p>
                        </div>
                        <div>
                            <p className="text-sm text-gray-500">Отчество</p>
                            <p className="text-lg">{profile.patronymic || "—"}</p>
                        </div>
                        <div>
                            <p className="text-sm text-gray-500">Телефон</p>
                            <p className="text-lg">{profile.phone}</p>
                        </div>
                    </div>
                ) : (
                    <form onSubmit={handleSave}>
                        {updateError && (
                            <div className="bg-red-100 border border-red-400 text-red-700 px-4 py-3 rounded mb-4">
                                {updateError}
                            </div>
                        )}

                        {updateSuccess && (
                            <div className="bg-green-100 border border-green-400 text-green-700 px-4 py-3 rounded mb-4">
                                {updateSuccess}
                            </div>
                        )}

                        <div className="grid grid-cols-1 md:grid-cols-2 gap-4 mb-4">
                            <div>
                                <label htmlFor="lastName" className="block text-sm font-medium text-gray-700 mb-1">
                                    Фамилия
                                </label>
                                <input
                                    type="text"
                                    name="lastName"
                                    id="lastName"
                                    value={formData.lastName}
                                    onChange={handleChange}
                                    className="w-full px-3 py-2 border border-gray-300 rounded-md focus:outline-none focus:ring-1 focus:ring-main"
                                    required
                                />
                            </div>
                            <div>
                                <label htmlFor="firstName" className="block text-sm font-medium text-gray-700 mb-1">
                                    Имя
                                </label>
                                <input
                                    type="text"
                                    name="firstName"
                                    id="firstName"
                                    value={formData.firstName}
                                    onChange={handleChange}
                                    className="w-full px-3 py-2 border border-gray-300 rounded-md focus:outline-none focus:ring-1 focus:ring-main"
                                    required
                                />
                            </div>
                            <div>
                                <label htmlFor="patronymic" className="block text-sm font-medium text-gray-700 mb-1">
                                    Отчество (необязательно)
                                </label>
                                <input
                                    type="text"
                                    name="patronymic"
                                    id="patronymic"
                                    value={formData.patronymic}
                                    onChange={handleChange}
                                    className="w-full px-3 py-2 border border-gray-300 rounded-md focus:outline-none focus:ring-1 focus:ring-main"
                                />
                            </div>
                            <div>
                                <label htmlFor="phone" className="block text-sm font-medium text-gray-700 mb-1">
                                    Номер телефона
                                </label>
                                <input
                                    type="tel"
                                    name="phone"
                                    id="phone"
                                    value={formatPhoneDisplay(formData.phone)}
                                    onChange={handleChange}
                                    className="mt-1 block w-full px-3 py-2 border border-gray-300 rounded-md shadow-sm focus:outline-none focus:ring-indigo-500 focus:border-indigo-500 sm:text-sm"
                                    placeholder="+7 (999) 123-45-67"
                                />
                            </div>
                        </div>

                        <div className="mt-4 mb-6">
                            <h3 className="text-lg font-medium mb-2">Изменение пароля (необязательно)</h3>
                            <div className="grid grid-cols-1 md:grid-cols-2 gap-4">
                                <div className="md:col-span-2">
                                    <label htmlFor="newPassword" className="block text-sm font-medium text-gray-700 mb-1">
                                        Новый пароль
                                    </label>
                                    <input
                                        type="password"
                                        name="newPassword"
                                        id="newPassword"
                                        value={formData.newPassword}
                                        onChange={handleChange}
                                        className="w-full px-3 py-2 border border-gray-300 rounded-md focus:outline-none focus:ring-1 focus:ring-main"
                                    />
                                </div>
                                <div className="md:col-span-2">
                                    <label htmlFor="newPasswordRepeat" className="block text-sm font-medium text-gray-700 mb-1">
                                        Повторите новый пароль
                                    </label>
                                    <input
                                        type="password"
                                        name="newPasswordRepeat"
                                        id="newPasswordRepeat"
                                        value={formData.newPasswordRepeat}
                                        onChange={handleChange}
                                        className="w-full px-3 py-2 border border-gray-300 rounded-md focus:outline-none focus:ring-1 focus:ring-main"
                                    />
                                </div>
                            </div>
                        </div>

                        <div className="border-t pt-4">
                            <label htmlFor="currentPassword" className="block text-sm font-medium text-gray-700 mb-1">
                                Текущий пароль (для подтверждения изменений)
                            </label>
                            <input
                                type="password"
                                name="currentPassword"
                                id="currentPassword"
                                value={formData.currentPassword}
                                onChange={handleChange}
                                className="w-full px-3 py-2 border border-gray-300 rounded-md focus:outline-none focus:ring-1 focus:ring-main"
                                required
                            />
                        </div>

                        <div className="flex gap-3 mt-6">
                            <button
                                type="submit"
                                className="px-4 py-2 bg-main text-white rounded hover:bg-main2 transition-colors"
                            >
                                Сохранить изменения
                            </button>
                            <button
                                type="button"
                                onClick={handleEditToggle}
                                className="px-4 py-2 bg-gray-200 text-gray-800 rounded hover:bg-gray-300 transition-colors"
                            >
                                Отмена
                            </button>
                        </div>
                    </form>
                )}
            </div>

            <div className="mt-8 grid grid-cols-1 md:grid-cols-3 gap-4">
                <Link href="/senior-admin/hospitals" className="bg-white p-4 shadow-md rounded-lg hover:shadow-lg transition-shadow text-center">
                    <h3 className="text-lg font-semibold text-main2 mb-2">Больницы</h3>
                    <p className="text-gray-600">Управление больницами</p>
                </Link>
                <Link href="/senior-admin/junior-admins" className="bg-white p-4 shadow-md rounded-lg hover:shadow-lg transition-shadow text-center">
                    <h3 className="text-lg font-semibold text-main2 mb-2">Администраторы</h3>
                    <p className="text-gray-600">Управление младшими администраторами</p>
                </Link>
                <Link href="/senior-admin/users" className="bg-white p-4 shadow-md rounded-lg hover:shadow-lg transition-shadow text-center">
                    <h3 className="text-lg font-semibold text-main2 mb-2">Пользователи</h3>
                    <p className="text-gray-600">Просмотр и управление пользователями</p>
                </Link>
            </div>

            <div className="flex justify-between mt-8">
                <Link href="/" className="text-main hover:text-main2 transition-colors">
                    ← Вернуться на главную
                </Link>
            </div>
        </div>
    );
}
